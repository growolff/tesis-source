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
    MOTOR_readSpeed(motors[0]);
    MOTOR_readSpeed(motors[1]);
    MOTOR_readSpeed(motors[2]);
}

/* [] END OF FILE */
