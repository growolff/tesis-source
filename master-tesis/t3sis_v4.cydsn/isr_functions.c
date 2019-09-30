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

CY_ISR(M1_HA_INT) //interrupt on level change in PM1_HA signal
{
    MOTOR_readSpeed(motors[0]);
}
CY_ISR(M2_HA_INT) //interrupt on level change in PM1_HA signal
{
    //MOTOR_readSpeed(motors[1]);
}

CY_ISR(SPD_COMMAND_INT)
{
    //MOTOR_setSpeed(motors[0],motors[0]->ref_spd);
    //MOTOR_setSpeed(motors[1]);
}

CY_ISR(RVT_COMMAND_INT)
{
    MOTOR_setPosition(motors[0]);
    //MOTOR_setPosition(motors[1]);
}

/* [] END OF FILE */
