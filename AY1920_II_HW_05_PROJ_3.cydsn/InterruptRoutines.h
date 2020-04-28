/* 
* \file InterruptRoutines.h
*/


#ifndef _INTERRUPT_ROUTINES_H_
   
    #define _INTERRUPT_ROUTINES_H_
    
    #include "project.h"
    #include "I2C_Interface.h"
    
    CY_ISR_PROTO(Custom_TIMER_ISR);
    
    /* Number of bytes in each packet to be transmitted: header; (from LSB to MSB) 4 bytes of X, 4 bytes for Y, 4 bytes for Z; tail. */
    #define TRANSMIT_BUFFER_SIZE 14
    
    extern uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE];    
    
    extern volatile uint8_t PacketReadyFlag;
#endif
/* [] END OF FILE */
