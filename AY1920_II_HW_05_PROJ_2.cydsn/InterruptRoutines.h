/* 
* \file InterruptRoutines.h
*/


#ifndef _INTERRUPT_ROUTINES_H_
   
    #define _INTERRUPT_ROUTINES_H_
    
    #include "project.h"
    #include "I2C_Interface.h"
    
    /* Number of bytes in each packet to be transmitted: header, X LSB, X MSB, Y LSB, Y MSB, Z LSB, Z MSB, tail */
    #define TRANSMIT_BUFFER_SIZE 8
    
    extern uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE];
    
    extern volatile uint8_t PacketReadyFlag;
    
    CY_ISR_PROTO(Custom_TIMER_ISR);
#endif
/* [] END OF FILE */
