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
uint8_t accelerometer_data[6];

/*Flag to start UART transmission in main*/
volatile uint8_t PacketReadyFlag = 0; 

void Data_Conversion(void) {
    //to define
}

void Packet_Preparation(void) {
    //to define
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
    
    uint8_t status_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  STATUS_REG_ADDR,
                                                  &status_reg);
    if (error == NO_ERROR) {
        if (status_reg & STATUS_REG_MASK) {
        
            error = I2C_Peripheral_ReadRegisterMulti( LIS3DH_DEVICE_ADDRESS,
                                                      OUT_X_L_ADDR,
                                                      6,
                                                      accelerometer_data);
            if (error == NO_ERROR) {
                Data_Conversion();
                Packet_Preparation();
                PacketReadyFlag = 1;
            }
        }
    }   
}
/* [] END OF FILE */
