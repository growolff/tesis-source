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
    if(motors[0]->control_mode == 0 || motors[0]->control_mode == 1){
        MOTOR_setSpdRef(motors[0],motors[0]->ref_spd);
        
        if(motors[0]->curr_spd < 300){
            motors[0]->spd_controller.iTerm = 0;
        }
        
        motors[0]->spdPID_out = PID_calculatePID(&motors[0]->spd_controller,motors[0]->curr_spd);
        SPEED_PWM_WriteCompare(fn_mapper(motors[0]->spdPID_out,0,8000,0,255));
    }
}

CY_ISR(RVT_COMMAND_INT)
{
    MOTOR_readRevolution(motors[0]); // read rotor current position
    if(motors[0]->control_mode == 1)
    {
        MOTOR_setRvtRef(motors[0]);
        
        motors[0]->rvtPID_out = PID_calculatePID(&motors[0]->rvt_controller,motors[0]->curr_rvt);
        
        (motors[0]->rvtPID_out > 0) ?  MOTOR_setPinDIR(motors[0],1) : MOTOR_setPinDIR(motors[0],0);

        //PM1.ref_spd = abs(PM1.rvtPID_out);
        motors[0]->ref_spd = fn_mapper(abs( motors[0]->rvtPID_out),0,255,0,8000);
       // SPEED_DAC_SetValue(PM1.ref_spd);
    }
 
}

/* [] END OF FILE */
