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
    PID_setMaxValue(&motor->rvt_controller, 255.0);
    PID_setMinValue(&motor->rvt_controller, -255.0);
/*
    PID_init(&motor->tns_controller,motor->tns_params[0],motor->tns_params[1],motor->tns_params[2]);
    PID_setMaxValue(&motor->tns_controller, 1300);
    PID_setMinValue(&motor->tns_controller, -1300);*/
   
    motor->DIR = pin_dir;
    motor->ENABLE = pin_enable;
    
    MOTOR_resetVariables(motor);

    MOTOR_setPinENABLE(motor,0);
    MOTOR_setPinDIR(motor,0);
    
    motor->readIndex = 0;
    motor->total = 0;
    motor->average = 0;
    
    for (int thisReading = 0; thisReading < 10; thisReading++) {
        motor->readings[thisReading] = 0;
    }
    
    motor->error_check_counter = 0;
    motor->control_mode = 0;
}

void MOTOR_resetVariables(MOTOR_t* motor)
{
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
    
    motor->control_mode = 1;
}

void MOTOR_setRvtControlParams(MOTOR_t* motor, float kp, float ki, float kd)
{
    motor->rvt_params[0] = kp;
    motor->rvt_params[1] = ki;
    motor->rvt_params[2] = kd;
    PID_setCoeffs(&motor->rvt_controller,kp,ki,kd);
}

void MOTOR_setTnsControlParams(MOTOR_t* motor, float kp, float ki, float kd)
{
    motor->tns_params[0] = kp;
    motor->tns_params[1] = ki;
    motor->tns_params[2] = kd;
    PID_setCoeffs(&motor->tns_controller,kp,ki,kd);
}

void MOTOR_initControlParams(MOTOR_t* motor, float* rvt, float* tns)
{
    motor->rvt_params[0] = rvt[0];
    motor->rvt_params[1] = rvt[1];
    motor->rvt_params[2] = rvt[2];
    
    motor->tns_params[0] = tns[0];
    motor->tns_params[1] = tns[1];
    motor->tns_params[2] = tns[2];
}

void MOTOR_externControl(MOTOR_t* motor, uint8 ctrl, int32 ref)
{
    MOTOR_setControlMode(motor,ctrl);
    switch(ctrl)
    {
        case 1:
            motor->ref_rvt = ref;
            break;
        case 2:
            motor->ref_spd = ref;
            break;
        case 3:
            motor->ref_tns = ref;
            break;
        default:
            break;
    }
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
    if(mode == 1){  // set the initial position if control mode is revolution
        motor->init_pos = motor->curr_rvt;
    }
    MOTOR_resetVariables(motor);
    
    motor->control_mode = mode;     // set the control mode
}

void MOTOR_commandDriver(MOTOR_t* motor, uint8 motor_number, uint8 speed_value)
{
    /*
    if (MOTOR_checkActuatorLimits(motor) != 0){
        MOTOR_setPinBRAKEn(motor);
        MOTOR_sendSpeedCommand(motor_number,speed_value);
    }
    else{
        MOTOR_clearPinBRAKEn(motor);
    }*/
    if (MOTOR_checkActuatorLimits(motor) != 0){
        MOTOR_sendSpeedCommand(motor_number,speed_value);
    }
    else{
        MOTOR_sendSpeedCommand(motor_number,speed_value);
    }
}

void MOTOR_sendSpeedCommand(uint8 motor_number, uint8 speed_value)
{
    switch( motor_number ){
        case 1:
            SPEED_DAC_SetValue(speed_value);
            break;
    }
}

void MOTOR_setRvtRef(MOTOR_t* motor)
{    
    PID_setRef(&motor->rvt_controller,motor->ref_rvt);
}

void MOTOR_setTnsRef(MOTOR_t* motor, int32_t tnsRef)
{    
    PID_setRef(&motor->tns_controller,tnsRef);
        
    motor->tnsPID_out = PID_calculatePID(&motor->tns_controller,motor->curr_tns);
}

int32 MOTOR_get_tension_g(MOTOR_t* motor, int16 tension)
{
    motor->total = motor->total - motor->readings[motor->readIndex];
    // read from the sensor:
    motor->readings[motor->readIndex] = tension;
    // add the reading to the total:
    motor->total = motor->total + motor->readings[motor->readIndex];
    // advance to the next position in the array:
    motor->readIndex = motor->readIndex + 1;

    // if we're at the end of the array...
    if (motor->readIndex >= NUMREADINGS) {
        // ...wrap around to the beginning:
        motor->readIndex = 0;
    }
    motor->average = motor->total/NUMREADINGS;
    
    return (int32)((5000.0/4096.0)*motor->average/0.0035)/1000;
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

/*
void MOTOR_readCurrentTension(MOTOR_t* motor, uint8 motor_number)
{
    if(ADC_TS_IsEndConversion(ADC_TS_RETURN_STATUS)!=0){
            TS_array = MOTOR_StoreADCResults();
    }
    switch(motor_number) {
        case 1:
            motor->curr_tns = MOTOR_get_tension_g(motor,TS_array[0]);
            break;
        case 2:
            motor->curr_tns = MOTOR_get_tension_g(motor,TS_array[1]);
            break;
    }
}*/

float MOTOR_getTR(MOTOR_t* motor, float alpha)
{
    float ax1 = motor->A + (motor->R * alpha);   
    return (sqrt((motor->L * motor->L) - (ax1 * ax1)) / (motor->R*ax1));
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
    if(setPin == 0){
        *motor->ENABLE.DR &= ~(motor->ENABLE.MASK);
        motor->ENABLE.STATE = 0; 
    }
    else if(setPin == 1){
        *motor->ENABLE.DR |= motor->ENABLE.MASK;
        motor->ENABLE.STATE = 1;
    }
    
}

/* [] END OF FILE */
