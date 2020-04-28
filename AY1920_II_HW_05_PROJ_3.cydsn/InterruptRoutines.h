/* 
* \file InterruptRoutines.h
*/


#ifndef _INTERRUPT_ROUTINES_H_
   
    #define _INTERRUPT_ROUTINES_H_
    
    #include "project.h"
    #include "I2C_Interface.h"
    
    CY_ISR_PROTO(Custom_TIMER_ISR);
    
    extern volatile uint8_t PacketReadyFlag;
#endif
/* [] END OF FILE */
