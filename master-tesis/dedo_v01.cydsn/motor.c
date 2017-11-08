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

void MOTOR_init(MOTOR_t* motor, PIN_t pin_enable, PIN_t pin_braken, PIN_t pin_dir)
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
    
    motor->BRAKEn = pin_braken;
    motor->DIR = pin_dir;
    motor->ENABLE = pin_enable;
    
    MOTOR_resetVariables(motor);
    
}

void MOTOR_resetVariables(MOTOR_t* motor)
{
    motor->spdPID_out = 0;
    motor->rvtPID_out = 0;
    motor->tnsPID_out = 0;
    
    motor->ref_spd = 0;
    motor->ref_rvt = 0;
    motor->ref_tns = 0;

    motor->curr_spd = 0;
    motor->curr_rvt = 0;
    motor->curr_tns = 0;
    
    motor->rvt_aux = 0;
    motor->rvt_last_count = 0;
    motor->period_ha = 0;
    motor->ma = 0;
    motor->ca = 0;
}

void MOTOR_setRvtControlParams(MOTOR_t* motor, float kp, float ki, float kd)
{
    motor->rvt_params[0] = kp;
    motor->rvt_params[1] = ki;
    motor->rvt_params[2] = kd;
    PID_setCoeffs(&motor->rvt_controller,kp,ki,kd);
}

void MOTOR_setSpdControlParams(MOTOR_t* motor, float kp, float ki, float kd)
{
    motor->spd_params[0] = kp;
    motor->spd_params[1] = ki;
    motor->spd_params[2] = kd;
    PID_setCoeffs(&motor->spd_controller,kp,ki,kd);
}

void MOTOR_initControlParams(MOTOR_t* motor, float* rvt, float* spd, float* tns)
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

/* Function: setControlMode
 * --------
 * set control mode for specific motor 
 *  Control modes are:
 *  1. Revolutions control
 *  2. Speed control
 *  3. Tension control
 */
void MOTOR_setControlMode(MOTOR_t* motor, uint8_t mode)
{
    MOTOR_clearPinBRAKEn(motor);    // stop the motor
    MOTOR_resetVariables(motor);
    if(mode == 1){  // set the initial position if control mode is revolution
        motor->init_pos = 0;
    }
    
    motor->control_mode = mode;     // set the control mode
}

void MOTOR_setSpdRef(MOTOR_t* motor, int32_t spdRef)
{
    PID_setRef(&motor->spd_controller,spdRef);
    
    if(motor->curr_spd == 0){
        motor->spd_controller.iTerm = 0;
    }
    motor->spdPID_out = PID_calculatePID(&motor->spd_controller,motor->curr_spd);
}

void MOTOR_setRvtRef(MOTOR_t* motor, int32_t rvtRef)
{
    PID_setRef(&motor->rvt_controller,(rvtRef) - (motor->init_pos));
        
    motor->rvtPID_out = PID_calculatePID(&motor->rvt_controller,motor->curr_rvt);
}

void MOTOR_ToggleHandBrake(MOTOR_t* motor)
{    
    if (motor->BRAKEn.STATE == 0){                 
        //PM1_BRAKEn_Write(motor->BRAKEn_state_);    // write to BRAKEn pin
        *motor->BRAKEn.DR |= motor->BRAKEn.MASK;
        motor->BRAKEn.STATE = 1;   // update motor status
    }
    else{
        //PM1_BRAKEn_Write(motor->BRAKEn_state_);
        *motor->BRAKEn.DR &= ~(motor->BRAKEn.MASK);
        motor->BRAKEn.STATE = 0; // update motor status
    }    
}

void MOTOR_clearPinBRAKEn(MOTOR_t * motor)
{
    *motor->BRAKEn.DR &= ~(motor->BRAKEn.MASK);
}
void MOTOR_clearPinDIR(MOTOR_t * motor)
{
    *motor->DIR.DR &= ~(motor->DIR.MASK);
}
void MOTOR_clearPinENABLE(MOTOR_t * motor)
{
    *motor->ENABLE.DR &= ~(motor->ENABLE.MASK);
}
void MOTOR_setPinBRAKEn(MOTOR_t * motor)
{
    *motor->BRAKEn.DR |= motor->BRAKEn.MASK;
}
void MOTOR_setPinDIR(MOTOR_t * motor)
{
    *motor->DIR.DR |= motor->DIR.MASK;    
}
void MOTOR_setPinENABLE(MOTOR_t * motor)
{
    *motor->ENABLE.DR |= motor->ENABLE.MASK;
}

/* [] END OF FILE */
