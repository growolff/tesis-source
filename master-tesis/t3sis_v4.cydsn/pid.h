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

#ifndef PID_H_
#define PID_H_

#define VAR_TYPE float

#include <stdint.h>
    
#define FLOAT_TO_INT_SCALE	(1 << 12)  //scale up by 4096
//#define FLOAT_TO_INT_SCALE	100  //scale up by 4096

typedef struct PID_t
{
    VAR_TYPE kP;
    VAR_TYPE kI;
    VAR_TYPE kD;
    VAR_TYPE refVal;
    VAR_TYPE inputValue;
    VAR_TYPE lastValue;
    VAR_TYPE iTerm;
    VAR_TYPE minValue;
    VAR_TYPE maxValue;
    VAR_TYPE scaledMin;
    VAR_TYPE scaledMax;
    VAR_TYPE dbg;
} PID_t;


void PID_init(PID_t* pid, float p, float i, float d);
void PID_setCoeffs(PID_t* pid, float p, float i, float d);
void PID_setRef(PID_t* pid, VAR_TYPE pnt);
void PID_setMinValue(PID_t* pid, VAR_TYPE mn);
void PID_setMaxValue(PID_t* pid, VAR_TYPE mx);
VAR_TYPE PID_calculatePID(PID_t* pid, VAR_TYPE input);

#endif

/* [] END OF FILE */
