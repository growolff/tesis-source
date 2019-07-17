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

CY_ISR(PM1_HA_INT) //interrupt on level change in PM1_HA signal
{
    MOTOR_readSpeed(motors[0]);
}

CY_ISR(SPD_COMMAND_INT)
{
    MOTOR_setSpeed(motors[0]);
}

CY_ISR(RVT_COMMAND_INT)
{
    MOTOR_setPosition(motors[0]);
}

/* [] END OF FILE */
