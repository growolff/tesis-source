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

void MOTOR_init(MOTOR_t* motor, PIN_t pin_enable, PIN_t pin_dir)
{
    /* initialize motor controllers */
    PID_init(&motor->spd_controller,motor->spd_pid[0],motor->spd_pid[1],motor->spd_pid[2]);
    PID_setMaxValue(&motor->spd_controller, _MOTOR_MAX_SPEED);
    PID_setMinValue(&motor->spd_controller, _MOTOR_MIN_SPEED);

    PID_init(&motor->rvt_controller,motor->rvt_pid[0],motor->rvt_pid[1],motor->rvt_pid[2]);
    PID_setMaxValue(&motor->rvt_controller, _MOTOR_MAX_POS);
    PID_setMinValue(&motor->rvt_controller, - _MOTOR_MAX_POS);

    motor->DIR = pin_dir;
    motor->ENABLE = pin_enable;

    MOTOR_setPinENABLE(motor,M_DISABLE);
    MOTOR_setPinDIR(motor,M_CCW);

    motor->last_count = 0;
    /*
    motor->readIndex = 0;
    motor->total = 0;
    motor->average = 0;

    for (int thisReading = 0; thisReading < 10; thisReading++) {
        motor->readings[thisReading] = 0;
    }
    */
}

void motor_echof(float data)
{
    sprintf(msg,"%.2f\r\n", data);
    UART_PutString(msg);
}

void motor_echod(int data)
{
    sprintf(msg,"%d\r\n", data);
    UART_PutString(msg);
}

void MOTOR_setSpeed(MOTOR_t* motor, int32_t ref)
{
    if(motor->control_mode == 0 || motor->control_mode == 1){
        MOTOR_setSpdRef(motor,ref);

        if(motor->curr_spd <= _MOTOR_MIN_SPEED){
            motor->spd_controller.iTerm = 0;
        }
        motor->spdPID_out = PID_calculatePID(&motor->spd_controller,motor->curr_spd);
        PWM_M1_WriteCompare(255*motor->spdPID_out/_MOTOR_MAX_SPEED);
    }
}

void MOTOR_setPosition(MOTOR_t* motor)
{
    motor->rvtPID_out = PID_calculatePID(&motor->rvt_controller,motor->curr_rvt);
    int16_t motor_PWM = motor->rvtPID_out;
    if(motor->rvtPID_out > 0){
        MOTOR_setCW(motor);
    }
    else{
        MOTOR_setCCW(motor);
        motor_PWM = -motor_PWM;
    }
    switch (motor->idx)
    {
        case 0:
            PWM_M1_WriteCompare(motor_PWM);
            break;
        case 1:
            PWM_M2_WriteCompare(motor_PWM);
            break;
    }

}

int16 MOTOR_getRvtCounter(MOTOR_t* motor)
{
    int16 counter = 0;
    switch(motor->idx)
    {
        case 0:
            counter = DC_M1_GetCounter();
            break;
        case 1:
            counter = DC_M2_GetCounter();
            break;/*
        case 2:
            counter = DC_M2_GetCounter();
            break;
        case 3:
            counter = DC_M2_GetCounter();
            break;*/
    }
    return counter;
}

uint32 MOTOR_getSpdCounter(MOTOR_t* motor)
{
    uint32 counter = 0;
    switch (motor->idx)
    {
        case 0:
            counter = HA_TIMER_M1_ReadCounter();
            HA_TIMER_M1_WriteCounter(0);
            break;
        case 1:
            counter = HA_TIMER_M2_ReadCounter();
            HA_TIMER_M2_WriteCounter(0);
            break;
    }
    return counter;
}

void MOTOR_resetSpdCounter(MOTOR_t* motor)
{
    switch (motor->idx)
    {
        case 0:
            HA_TIMER_M1_WriteCounter(0);
            break;
        case 1:
            HA_TIMER_M2_WriteCounter(0);
            break;
    }
}

void MOTOR_setControlMode(MOTOR_t* motor, uint8_t mode)
{
    if(motor->ENABLE.STATE == 1){
        // disable motor if enabled
        MOTOR_setDisable(motor);
        if(mode == M_POSITION_CONTROL_MODE){
            motor->init_pos = MOTOR_getRvtCounter(motor);
        }
        motor->control_mode = mode;
        // enable motor
        MOTOR_setEnable(motor);
    }
    else {
        if(mode == M_POSITION_CONTROL_MODE){
            motor->init_pos = MOTOR_getRvtCounter(motor);
        }
        motor->control_mode = mode;
    }
}

void MOTOR_setRvtControlParams(MOTOR_t* motor, float kp, float ki, float kd)
{
    motor->rvt_pid[0] = kp;
    motor->rvt_pid[1] = ki;
    motor->rvt_pid[2] = kd;

    PID_setCoeffs(&motor->rvt_controller,kp,ki,kd);
}

void MOTOR_setSpdControlParams(MOTOR_t* motor, float kp, float ki, float kd)
{
    motor->spd_pid[0] = kp;
    motor->spd_pid[1] = ki;
    motor->spd_pid[2] = kd;

    PID_setCoeffs(&motor->spd_controller,kp,ki,kd);
}

void MOTOR_setRvtRef(MOTOR_t* motor, int16_t ref_rvt)
{
    PID_setRef(&motor->rvt_controller,ref_rvt);
    motor->ref_rvt = ref_rvt;
}

void MOTOR_readSpeed(MOTOR_t* motor)
{
    /*
    motor->ca = MOTOR_getSpdCounter(motor);
    MOTOR_resetSpdCounter(motor);

    motor->period_ha = -1*(motor->ca+motor->ma);
    motor->ma = motor->ca;

    if(MOTOR_checkDir(motor) != -1){
        motor->curr_spd = (SPD_MEASUREMENT_FREQ_CLOCK/motor->period_ha) * 30;
    }
    else{
        motor->curr_spd = 0;
    }
    */

    int16 counter = MOTOR_getRvtCounter(motor);
    motor->curr_rvt = (counter - motor->init_pos);

    int16 diff = counter - motor->last_count;

    if(counter >= 0 || counter <= 5000){
        if (motor->DIR.STATE == M_CW){
            motor->curr_spd = (diff)*60*20/10;
            motor->curr_dir = M_CW;
        }
        else if (motor->DIR.STATE == M_CCW){
            motor->curr_spd = -1*(diff)*60*20/10;
            motor->curr_dir = M_CCW;
        }
    }
    else{
        counter = motor->last_count;
    }
    motor->last_count = counter;

    if(motor->curr_spd == 0){
        motor->curr_dir = M_STOP;
    }
}

int16 * MOTOR_StoreADCResults() // store ADC conversion result in a sensor data array
{
	uint16 i;
    static int16 dest[NUM_SENSORS];

	for (i = 0; i < NUM_SENSORS; i++) {
		//dest[i] = ADC_TS_GetResult16(i);
	}
    return dest;
}

void MOTOR_setPinDIR(MOTOR_t * motor, uint8_t setPin)
{
    if(setPin == M_CCW){
        *motor->DIR.DR &= ~(motor->DIR.MASK);
        motor->DIR.STATE = M_CCW;
    }
    else if(setPin == M_CW){
        *motor->DIR.DR |= motor->DIR.MASK;
        motor->DIR.STATE = M_CW;
    }
}

void MOTOR_setCCW(MOTOR_t * motor)
{
    if(motor->DIR.STATE == M_CW){
        *motor->DIR.DR &= ~(motor->DIR.MASK);
        motor->DIR.STATE = M_CCW;   
    }
}

void MOTOR_setCW(MOTOR_t * motor)
{
    if(motor->DIR.STATE == M_CCW){
        *motor->DIR.DR |= motor->DIR.MASK;
        motor->DIR.STATE = M_CW;
    }
}

void MOTOR_setPinENABLE(MOTOR_t * motor, uint8_t setPin)
{
    if(setPin == M_DISABLE){ // turn off
        *motor->ENABLE.DR &= ~(motor->ENABLE.MASK);
        motor->ENABLE.STATE = M_DISABLE;
    }
    else if(setPin == M_ENABLE){ // turn on
        *motor->ENABLE.DR |= motor->ENABLE.MASK;
        motor->ENABLE.STATE = M_ENABLE;
    }

}

void MOTOR_setEnable(MOTOR_t *motor){
    *motor->ENABLE.DR |= motor->ENABLE.MASK;
    motor->ENABLE.STATE = M_ENABLE;
}

void MOTOR_setDisable(MOTOR_t *motor){
    *motor->ENABLE.DR &= ~(motor->ENABLE.MASK);
    motor->ENABLE.STATE = M_DISABLE;
}

/* [] END OF FILE */
