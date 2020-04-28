/* 
* \file InterruptRoutines.c
*/

/* Include header files */
#include "InterruptRoutines.h"
#include "registers.h"

/* mask for bit 3 of the status register that is set when new data are available on all 3 axis */
#define STATUS_REG_MASK 0x08 

/* number of bytes of data read from the slave device: LSB and MSB of X,Y and Z axis acceleration */
#define DATA_BYTES_NUMBER 6


/* array to store accelerometer output data (starting from the position zero: LSB and MSB of X,Y and Z axis acceleration) */
uint8_t AccelerometerData[DATA_BYTES_NUMBER];

/* array to store the 3 accelerations in mg (from the position zero: X axis, Y axis, Z axis */
int16_t Accelerations_mg[DATA_BYTES_NUMBER/2];

/*Flag to start UART transmission in main*/
volatile uint8_t PacketReadyFlag = 0; 

/* Array of bytes to be trasmitted */
uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE]; 

/*
* \brief for each axis, starting from left-adjusted LSB and MSB, create a right-justified 16-bit integer corresponding to the acceleration in mg
*/
void Data_Conversion(void) {
    
    uint8_t i;
    
    for(i = 0; i < DATA_BYTES_NUMBER/2; i++) {
        
        /* right shift of 4 = right shift of 6 to get right-justified 10 bits + left shift of 2 to multiply by 4 and get the value in mg */
        Accelerations_mg[i] = (int16)((AccelerometerData[i*2] | (AccelerometerData[i*2+1] << 8))) >> 4;
        
    }

}

/* 
* \brief for each axis, put LSB and MSB of the acceleration value in mg in the correct position of the packet to be sent 
*/
void Packet_Preparation(void) {
    
    uint8_t i;
    
    for(i = 0; i<DATA_BYTES_NUMBER/2; i++) {
        
        DataBuffer[i*2+1] = Accelerations_mg[i] & 0xFF;
        DataBuffer[i*2+2] = Accelerations_mg[i] >> 8;
        
    }
        
}

/*
* \brief ISR on TC (every 10 ms):
* \read the status register
* \if bit 3 is set, read output data of all axis
* \convert the data
* \prepare the packet
* \rise the flag to send the packet 
*/
CY_ISR(Custom_TIMER_ISR) {
    
    Timer_ReadStatusRegister();
    
    uint8_t status_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  STATUS_REG_ADDR,
                                                  &status_reg);
    if (error == NO_ERROR) {
        if (status_reg & STATUS_REG_MASK) {
        
            error = I2C_Peripheral_ReadRegisterMulti( LIS3DH_DEVICE_ADDRESS,
                                                      OUT_X_L_ADDR,
                                                      6,
                                                      AccelerometerData);
            if (error == NO_ERROR) {
                Data_Conversion();
                Packet_Preparation();
                PacketReadyFlag = 1;
            }
        }
    }  
}
/* [] END OF FILE */
