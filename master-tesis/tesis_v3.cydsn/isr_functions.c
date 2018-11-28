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

CY_ISR(RVT_COMMAND_INT)
{
    MOTOR_setRvtRef(motors[0]);
    
    MOTOR_readRevolution(motors[0]); // read rotor current position    
    PM1.rvtPID_out = PID_calculatePID(&PM1.rvt_controller,PM1.curr_rvt);
    
    (PM1.rvtPID_out > 0) ?  MOTOR_setPinDIR(&PM1,1) : MOTOR_setPinDIR(&PM1,0);

    PM1.ref_spd = fabs(PM1.rvtPID_out);
    SPEED_DAC_SetValue(PM1.ref_spd);
}

/* [] END OF FILE */
