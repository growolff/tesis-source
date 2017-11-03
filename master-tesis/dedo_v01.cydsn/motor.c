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

#include "motor.h"

void MOTOR_init(MOTOR_t* motor)
{    
    /* initialize motor controllers */
    PID_init(&motor->spd_controller,motor->spd_params[0],motor->spd_params[1],motor->spd_params[2]);
    PID_setMaxValue(&motor->spd_controller, 10000);
    PID_setMinValue(&motor->spd_controller, 0);
    
    PID_init(&motor->rvt_controller,motor->rvt_params[0],motor->rvt_params[1],motor->rvt_params[2]);
    PID_setMaxValue(&motor->rvt_controller, 500);
    PID_setMinValue(&motor->rvt_controller, -500);

    PID_init(&motor->tns_controller,motor->tns_params[0],motor->tns_params[1],motor->tns_params[2]);
    PID_setMaxValue(&motor->tns_controller, 4096);
    PID_setMinValue(&motor->tns_controller, -4096);
    
    /* Initialize motor hardware */
    
}

void MOTOR_setControlParams(MOTOR_t* motor, float* rvt, float* spd, float* tns)
{
    motor->rvt_params[0] = rvt[0];
    motor->rvt_params[1] = rvt[1];
    motor->rvt_params[2] = rvt[2];
    
    motor->spd_params[0] = spd[0];
    motor->spd_params[1] = spd[1];
    motor->spd_params[2] = spd[2];
    
    motor->tns_params[0] = tns[0];
    motor->tns_params[1] = tns[1];
    motor->tns_params[2] = tns[2];
}

void MOTOR_setSpdRef(MOTOR_t* motor, int32_t spdRef)
{
    PID_setRef(&motor->spd_controller,spdRef);
    
    if(motor->curr_spd == 0){
        motor->spd_controller.iTerm = 0;
    }
    motor->spdOutPID = PID_calculatePID(&motor->spd_controller,motor->curr_spd);
    
    // maps the pid_output to a 8bit number for pwm control of motor
    //_pid_out = fn_mapper_8b(motor->spdOutPID,0,10000,0,255);
    //debug = _pid_out;

    #if defined(ANGLE_CONTROL) || defined(SPEED_CONTROL)
            VDAC8_Speed_SetValue((uint8)(fn_mapper_8b(motor->spdOutPID,0,10000,0,255)));
    #endif   
    
}


/* [] END OF FILE */
