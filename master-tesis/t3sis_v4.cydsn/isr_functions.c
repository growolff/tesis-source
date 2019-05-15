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
    MOTOR_readRevolution(motors[0]); // read rotor current position
    if(motors[0]->control_mode == 1)
    {
        MOTOR_setRvtRef(motors[0]);
        
        PM1.rvtPID_out = PID_calculatePID(&PM1.rvt_controller,PM1.curr_rvt);
        
        (PM1.rvtPID_out > 0) ?  MOTOR_setPinDIR(&PM1,1) : MOTOR_setPinDIR(&PM1,0);

        PM1.ref_spd = abs(PM1.rvtPID_out);
       // SPEED_DAC_SetValue(PM1.ref_spd);
    }
 
}

CY_ISR(SPD_COMMAND_INT)
{
    if(motors[0]->control_mode == 0){
        MOTOR_setSpdRef(motors[0],motors[0]->ref_spd);
        
        if(motors[0]->curr_spd < 300){
            motors[0]->spd_controller.iTerm = 0;
        }
        
        motors[0]->spdPID_out = PID_calculatePID(&motors[0]->spd_controller,motors[0]->curr_spd);
        //motors[0]->spdPID_out = fn_mapper_8b(motors[0]->spdPID_out,0,9000,0,255);
        /*

        PM1.ref_spd = abs(PM1.rvtPID_out);
        //SPEED_DAC_SetValue(PM1.ref_spd);
        SPEED_DAC_SetValue(10);
     
        */
        //SPEED_DAC_SetValue(255);
        //SPEED_DAC_SetValue(fn_mapper(motors[0]->spdPID_out,0,9000,0,255));
        SPEED_PWM_WriteCompare(fn_mapper(motors[0]->spdPID_out,0,9000,0,255));
    }
}

/* [] END OF FILE */
