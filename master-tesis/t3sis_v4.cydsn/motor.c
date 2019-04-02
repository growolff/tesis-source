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
    
    PID_init(&motor->rvt_controller,motor->rvt_params[0],motor->rvt_params[1],motor->rvt_params[2]);
    PID_setMaxValue(&motor->rvt_controller, 255);
    PID_setMinValue(&motor->rvt_controller, -255);
   
    motor->DIR = pin_dir;
    motor->ENABLE = pin_enable;

    MOTOR_setPinENABLE(motor,0);
    MOTOR_setPinDIR(motor,0);
    
    motor->readIndex = 0;
    motor->total = 0;
    motor->average = 0;
    
    for (int thisReading = 0; thisReading < 10; thisReading++) {
        motor->readings[thisReading] = 0;
    }
}

void MOTOR_setControlMode(MOTOR_t* motor, uint8_t mode)
{
    motor->control_mode = mode;
}

void MOTOR_initControlParams(MOTOR_t* motor, float* rvt)
{
    motor->rvt_params[0] = rvt[0];
    motor->rvt_params[1] = rvt[1];
    motor->rvt_params[2] = rvt[2];
}

void MOTOR_setRvtControlParams(MOTOR_t* motor, float kp, float ki, float kd)
{
    motor->rvt_params[0] = kp;
    motor->rvt_params[1] = ki;
    motor->rvt_params[2] = kd;
    PID_setCoeffs(&motor->rvt_controller,kp,ki,kd);
}

void MOTOR_sendSpeedCommand(uint8 speed_value)
{
    SPEED_DAC_SetValue(speed_value);
}

void MOTOR_setRvtRef(MOTOR_t* motor)
{    
    PID_setRef(&motor->rvt_controller,motor->ref_rvt);
}

void MOTOR_readSpeed(MOTOR_t* motor)
{
    motor->ca = PM1_HA_TIMER_ReadCounter();
    PM1_HA_TIMER_WriteCounter(0);
         
    motor->period_ha = -1*(motor->ca+motor->ma);
    motor->ma = motor->ca;
    motor->curr_spd = (HIGH_FREQ_CLOCK/motor->period_ha) * 30;
    
}

void MOTOR_readRevolution(MOTOR_t* motor)
{
    motor->rvt_aux = PM1_DirCounter_GetCounter();
    motor->curr_rvt = motor->rvt_aux - motor->init_pos;
}

void MOTOR_checkDir(MOTOR_t* motor, uint8 motor_number)
{
    switch(motor_number) {
        case 1:
            motor->rvt_aux = PM1_DirCounter_GetCounter();
            break;
    }
    if ( motor->rvt_aux - motor->rvt_last_count > 0 )
    {
        // if > 0 its turning right (CW) 
        motor->curr_dir = 1;
    }
    else if ( motor->rvt_aux - motor->rvt_last_count < 0 )
    {
        // if < 0 its turning left (CCW)
        motor->curr_dir = -1;
        //motor->curr_rvt = motor->curr_rvt < motor->MIN_RVT * 4 ? motor->MIN_RVT * 4 : motor->curr_rvt ;
    }
    else if ( motor->rvt_aux - motor->rvt_last_count == 0 )
    {
        // if = 0 its not rotating
        motor->curr_dir = 0;
        motor->curr_spd = 0;
    }
    // update variable
    motor->rvt_last_count = motor->rvt_aux;
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

void MOTOR_ToggleDir(MOTOR_t* motor)
{    
    if (motor->DIR.STATE == 0){   
        MOTOR_setPinDIR(motor,1);
    }
    else{
        MOTOR_clearPinDIR(motor);
    }    
}

void MOTOR_clearPinDIR(MOTOR_t * motor)
{
    *motor->DIR.DR &= ~(motor->DIR.MASK);
    motor->DIR.STATE = 0;
}
void MOTOR_clearPinENABLE(MOTOR_t * motor)
{
    *motor->ENABLE.DR &= ~(motor->ENABLE.MASK);
    motor->ENABLE.STATE = 0;
}
void MOTOR_setPinDIR(MOTOR_t * motor, uint8_t setPin)
{
    if(setPin == 0){
        *motor->DIR.DR &= ~(motor->DIR.MASK);
        motor->DIR.STATE = 0;  
    }
    else if(setPin == 1){
        *motor->DIR.DR |= motor->DIR.MASK;    
        motor->DIR.STATE = 1;
    }
}
void MOTOR_setPinENABLE(MOTOR_t * motor, uint8_t setPin)
{
    if(setPin == 0){ // turn off
        *motor->ENABLE.DR &= ~(motor->ENABLE.MASK);
        motor->ENABLE.STATE = 0; 
    }
    else if(setPin == 1){ // turn on
        *motor->ENABLE.DR |= motor->ENABLE.MASK;
        motor->ENABLE.STATE = 1;
    }
    
}

/* [] END OF FILE */
