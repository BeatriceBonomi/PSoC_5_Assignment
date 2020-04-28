/*
* \file registers.h
* \this file contains, for all the registers of the LIS3DH accelerometer used in this project:
* \-the definition of the register address
* \-the definition of the content written in the register (if the register is written)
*/
#ifndef _REGISTERS_H_ 
    
    #define _REGISTERS_H_
    
    /*address of the LIS3DH slave device */
    #define LIS3DH_DEVICE_ADDRESS 0x18
    
    /* STATUS_REGISTER (READ ONLY): bit 3 (ZYXDA) is read in the TIMER_ISR to check if new data are available for all 3 axis */
    #define STATUS_REG_ADDR 0x27 
    
    /* OUT LOW AND HIGH REGISTERS (READ ONLY) contain, for each axis, the 12 bit output expressed as two's complement left-justified */
    #define OUT_X_L_ADDR 0x28
    #define OUT_X_H_ADDR 0x29
    
    #define OUT_Y_L_ADDR 0x2A
    #define OUT_Y_H_ADDR 0x2B
    
    #define OUT_Z_L_ADDR 0x2C
    #define OUT_Z_H_ADDR 0x2D
    
    /*
    * \CONTROL REGISTER 1 (READ AND WRITE) configured in this way:
    * \-ctrl_reg_1[7:4] = 0101 --> data rate output set to 100 Hz
    * \-ctrl_reg_1[3] = 0      --> low power mode disabled
    * \-ctrl_reg_1[2] = 1      --> Z axis enabled
    * \-ctrl_reg_1[1] = 1      --> Y axis enabled    
    * \-ctrl_reg_1[0] = 1      --> X axis enabled    
    */
    #define CTRL_REG_1_ADDR 0x20
    #define CTRL_REG_1_CONTENT 0x57
    
    /*
    * \CONTROL REGISTER 4 (READ AND WRITE) configured in this way:
    * \-ctrl_reg_4[7] = 0    --> continuous update of output registers
    * \-ctrl_reg_4[6] = 0    --> data LSB at lower register address, data MSB at higher address
    * \-ctrl_reg_4[5:4] = 01 --> full scale range ± 4g
    * \-ctrl_reg_4[3] = 1    --> high resolution mode enabled   
    * \-ctrl_reg_4[2:1] = 00 --> self-test disabled
    * \-ctrl_reg_4[0] = 0    --> default value for SPI mode (don't care value for this bit since SPI is not used in this project) 
    */
    #define CTRL_REG_4_ADDR 0x23
    #define CTRL_REG_4_CONTENT 0x18    
#endif

/* [] END OF FILE */
