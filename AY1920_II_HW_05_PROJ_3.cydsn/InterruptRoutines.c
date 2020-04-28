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

/* Sensitivity in the case of 12-bit resolution and ±4g FSR : 2 mg/digit (from the accelerometer datasheet) */
#define SENSITIVITY 2

/* Conversion factor from acceleration in mg to acceleration in m/s^2 */
#define CONVERSION_FACTOR_MG_TO_MS2 0.00981


/* array to store accelerometer output data (starting from the position zero: LSB and MSB of X,Y and Z axis acceleration) */
uint8_t AccelerometerData[DATA_BYTES_NUMBER];

/* array to store the 3 accelerations in digit(from the position zero: X axis, Y axis, Z axis) */
int16_t Accelerations_digit[DATA_BYTES_NUMBER/2];

/* array to store the 3 accelerations in mg (from the position zero: X axis, Y axis, Z axis) */
int16_t Accelerations_mg[DATA_BYTES_NUMBER/2];

/* array to store the 3 accelerations in m/s^2 (from the position zero: X axis, Y axis, Z axis) */
float Accelerations_ms2[DATA_BYTES_NUMBER/2];

/* array to store the 3 accelerations in m/s^2 casted to int(from the position zero: X axis, Y axis, Z axis) */
int32_t Accelerations_ms2_int[DATA_BYTES_NUMBER/2];

/*Flag to start UART transmission in main*/
volatile uint8_t PacketReadyFlag = 0; 

/* Array of bytes to be trasmitted */
uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE]; 

/*
* \brief for each axis, starting from left-adjusted LSB and MSB:
* \-create a right-justified 16-bit integer corresponding to the acceleration in digit
* \-convert the acceleration in digit to acceleration in mg
* \-convert the acceleration in mg to acceleration in m/s^2
*/
void Data_Conversion(void) {
    
    uint8_t i;
    
    for(i = 0; i < DATA_BYTES_NUMBER/2; i++) {
        
        /* right shift of 4 to get right-justified 12 bits */
        Accelerations_digit[i] = (int16)((AccelerometerData[i*2] | (AccelerometerData[i*2+1] << 8))) >> 4;
        /* multiply by the sensitivity to get the value in mg */
        Accelerations_mg[i] = Accelerations_digit[i] * SENSITIVITY;
        /* convert the value in mg in m/s^2 */
        Accelerations_ms2[i] = ((float) Accelerations_mg[i]) * CONVERSION_FACTOR_MG_TO_MS2;
    }

}

/* 
* \brief for each axis, starting from the acceleration in m/s^2:
* \-cast to int32 after multiplying by 1000 to keep the first 3 decimals
* \-put the 4 bytes of the int32 in the correct position of the packet to be sent 
*/
void Packet_Preparation(void) {
    
    uint8_t i;
    
    for(i = 0; i<DATA_BYTES_NUMBER/2; i++) {
        
        Accelerations_ms2_int[i] = (Accelerations_ms2[i] * 1000.0);
        DataBuffer[i*4+1] = Accelerations_ms2_int[i] & 0xFF; /*LSB */
        DataBuffer[i*4+2] = (Accelerations_ms2_int[i] >> 8) & 0xFF; /* Second byte */
        DataBuffer[i*4+3] = (Accelerations_ms2_int[i] >> 16) & 0xFF; /* Third byte */
        DataBuffer[i*4+4] = (Accelerations_ms2_int[i] >> 24); /* MSB */        
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
