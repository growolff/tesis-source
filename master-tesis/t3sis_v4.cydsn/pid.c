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
#include "pid.h"

void PID_init(PID_t* pid, float p, float i, float d)
{
    #if VAR_TYPE==int32_t
    pid->kP = (int32_t)(p * FLOAT_TO_INT_SCALE);
    pid->kI = (int32_t)(i * FLOAT_TO_INT_SCALE);
    pid->kD = (int32_t)(d * FLOAT_TO_INT_SCALE);
    #endif
    
    #if VAR_TYPE==float
    pid->kP = p;
    pid->kI = i;
    pid->kD = d;
    #endif
    
    pid->refVal = 0;
    pid->inputValue = 0;
    pid->iTerm = 0;
    pid->lastValue = 0;
}
    
void PID_setCoeffs(PID_t* pid, float p, float i, float d)
{
    #if VAR_TYPE==int32_t
    pid->kP = (int32_t)(p * FLOAT_TO_INT_SCALE);
    pid->kI = (int32_t)(i * FLOAT_TO_INT_SCALE);
    pid->kD = (int32_t)(d * FLOAT_TO_INT_SCALE);
    #endif
    
    #if VAR_TYPE==float
    pid->kP = p;
    pid->kI = i;
    pid->kD = d;
    #endif
    
}

void PID_setRef(PID_t* pid, VAR_TYPE pnt)
{
    pid->refVal = pnt;
}
void PID_setMinValue(PID_t* pid, VAR_TYPE mn)
{
    pid->minValue = mn;
    pid->scaledMin = mn * FLOAT_TO_INT_SCALE;
}

void PID_setMaxValue(PID_t* pid, VAR_TYPE mx)
{
    pid->maxValue = mx;
    pid->scaledMax = mx * FLOAT_TO_INT_SCALE;
}

VAR_TYPE PID_calculatePID(PID_t *pid, VAR_TYPE input)
{   
    VAR_TYPE error, diffInput, output;
 
    pid->inputValue = input;
    error =  pid->refVal - pid->inputValue;
    pid->iTerm += (pid->kI * error); //remember, all k* variables are scaled up

    if (pid->iTerm > pid->scaledMax) pid->iTerm = pid->scaledMax;
    if (pid->iTerm < pid->scaledMin) pid->iTerm = pid->scaledMin;

    diffInput = (pid->inputValue - pid->lastValue);

    //it's traditional to try to avoid using division in microcontrollers but the
    //SAM3X has a very fast hardware divider anyway. Besides, FLOAT_TO_INT_SCALE is
    //an even power of 2 so it can be done via a shift which is even faster.
    //output = ((pid->kP * error) + (pid->iTerm) - (pid->kD * diffInput)) / FLOAT_TO_INT_SCALE;
    
    //output = (pid->kP * error) / FLOAT_TO_INT_SCALE;
    output = (pid->kP * error);
    
    pid->dbg = error;
    
    if (output > pid->maxValue) output = pid->maxValue;
    if (output < pid->minValue) output = pid->minValue;

    pid->lastValue = pid->inputValue;
    
    return output;
}

/* [] END OF FILE */
