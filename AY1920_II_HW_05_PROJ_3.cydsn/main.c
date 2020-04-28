/**
* \brief Main source file for the I2C-Master project.
*
* \author Beatrice Bonomi
* \date april 28, 2020
*/

// Include required header files
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "registers.h"
#include "InterruptRoutines.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* start components through API */
    I2C_Peripheral_Start();
    UART_Debug_Start();
    
    CyDelay(5); /* "The boot procedure is complete about 5 milliseconds after device power-up." */
    
    /* String to print out messages on the UART */
    char message[50];
    
    /* variable to check error in I2C communication */
    ErrorCode error;    

    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    
    /* Configuration of control registers 1 and 4 to output 3 Axis accelerometer data in High Resolution Mode at 100 Hz in the ±4.0 g FSR. */
    /* See registers.h for single bits meaning */
    /* Both register are first read and then, only if their content is not the desired one, they are overwritten and read again to check correct writing */
    /* Read Control Register 1 */
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        CTRL_REG_1_ADDR,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /* Write Control Register 1 (only if the read content is not the desired one) */
    if (ctrl_reg1 != CTRL_REG_1_CONTENT)  
    {
        UART_Debug_PutString("\r\nWriting new values..\r\n");
        
        ctrl_reg1 = CTRL_REG_1_CONTENT;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             CTRL_REG_1_ADDR,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    
    
        /* Read Control Register 1 again to check for correct writing */
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            CTRL_REG_1_ADDR,
                                            &ctrl_reg1);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
        }
    }
    
    /* Read Control Register 4 */
    uint8_t ctrl_reg4; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        CTRL_REG_4_ADDR,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 4\r\n");   
    }
    
    /* Write Control Register 4 (only if the read content is not the desired one) */    
    if (ctrl_reg4 != CTRL_REG_4_CONTENT)
    {
        UART_Debug_PutString("\r\nWriting new values..\r\n");
        
        ctrl_reg4 = CTRL_REG_4_CONTENT;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             CTRL_REG_4_ADDR,
                                             ctrl_reg4);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 successfully written as: 0x%02X\r\n", ctrl_reg4);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 4\r\n");   
        }
    
    
        /* Read Control Register 4 again to check for correct writing */
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            CTRL_REG_4_ADDR,
                                            &ctrl_reg4);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 after overwrite operation: 0x%02X\r\n", ctrl_reg4);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to read control register 4\r\n");   
        }
    }
    
    /* set up header and tail */
    DataBuffer[0] = 0xA0;
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;
    
    Timer_Start();

    /* associate the interrupt to the correct ISR vector */
    Timer_isr_StartEx(Custom_TIMER_ISR);
    
    
    /* check continuously if a packet is ready to be transmitted 
    *  since DataBuffer content is modified by the ISR, time between 2 interrupts must be enough to complete the packet trasmission 
    *  see TopDesign for the baudrate choice
    */
    for(;;)
    {
        if (PacketReadyFlag) {
            UART_Debug_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE); /*API to transmit an array of bytes */
            PacketReadyFlag = 0;
        }
    }
}

/* [] END OF FILE */
