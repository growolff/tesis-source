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
#include "pid_o.h"

void PID_init(PID_t* pid, float p, float i, float d)
{
    pid->kP = (int16_t)(p * FLOAT_TO_INT_SCALE);
    pid->kI = (int16_t)(i * FLOAT_TO_INT_SCALE);
    pid->kD = (int16_t)(d * FLOAT_TO_INT_SCALE);
    pid->refVal = 0;
    pid->inputValue = 0;
    pid->iTerm = 0;
    pid->lastValue = 0;
}

void PID_setCoeffs(PID_t* pid, float p, float i, float d)
{
    pid->kP = (int16_t)(p * FLOAT_TO_INT_SCALE);
    pid->kI = (int16_t)(i * FLOAT_TO_INT_SCALE);
    pid->kD = (int16_t)(d * FLOAT_TO_INT_SCALE);
}

void PID_setRef(PID_t* pid, int16_t pnt)
{
    pid->refVal = pnt;
}
void PID_setMinValue(PID_t* pid, int16_t mn)
{
    pid->minValue = mn;
    pid->scaledMin = mn * FLOAT_TO_INT_SCALE;
}

void PID_setMaxValue(PID_t* pid, int16_t mx)
{
    pid->maxValue = mx;
    pid->scaledMax = mx * FLOAT_TO_INT_SCALE;
}

int16_t PID_calculatePID(PID_t* pid, int16_t input)
{
    int16_t output, error, diffInput;

    pid->inputValue = input;
    error = pid->refVal - pid->inputValue;
    pid->iTerm += (pid->kI * error); //remember, all k* variables are scaled up

    if (pid->iTerm > pid->scaledMax) pid->iTerm = pid->scaledMax;
    if (pid->iTerm < pid->scaledMin) pid->iTerm = pid->scaledMin;

    diffInput = (pid->inputValue - pid->lastValue);

    //it's traditional to try to avoid using division in microcontrollers but the
    //SAM3X has a very fast hardware divider anyway. Besides, FLOAT_TO_INT_SCALE is
    //an even power of 2 so it can be done via a shift which is even faster.
    output = ((pid->kP * error) + (pid->iTerm) - (pid->kD * diffInput)) / FLOAT_TO_INT_SCALE;

    pid->dbg = pid->iTerm;
    
    if (output > pid->maxValue) output = pid->maxValue;
    if (output < pid->minValue) output = pid->minValue;

    pid->lastValue = pid->inputValue;

    return output;
}

/* [] END OF FILE */
