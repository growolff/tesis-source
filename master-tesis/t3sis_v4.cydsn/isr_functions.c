/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "isr_functions.h"

CY_ISR(SPD_M2_INT) //interrupt at 20hz signal
{
    /*
    MOTOR_readSpeed(motors[0]);
    MOTOR_readSpeed(motors[1]);
    MOTOR_readSpeed(motors[2]);
    */
}

CY_ISR(M6_HA_INT){
    //ha_st = !ha_st;
    m6_counter += M6_HB_PIN_Read() ? 1 : -1;
    MOTOR_updateCounter(&M6, m6_counter);
}

/* [] END OF FILE */
