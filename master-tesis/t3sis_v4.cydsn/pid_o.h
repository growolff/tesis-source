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

#include <stdint.h>
    
#define FLOAT_TO_INT_SCALE	(1 << 12)  //scale up by 4096

typedef struct PID_t
{
    int16_t kP;
    int16_t kI;
    int16_t kD;
    int16_t refVal;
    int16_t inputValue;
    int16_t lastValue;
    int16_t iTerm;
    int16_t minValue;
    int16_t maxValue;
    int16_t scaledMin;
    int16_t scaledMax;
    int16_t dbg;
} PID_t;


void PID_init(PID_t* pid, float p, float i, float d);
void PID_setCoeffs(PID_t* pid, float p, float i, float d);
void PID_setRef(PID_t* pid, int16_t pnt);
void PID_setMinValue(PID_t* pid, int16_t mn);
void PID_setMaxValue(PID_t* pid, int16_t mx);
int16_t PID_calculatePID(PID_t* pid, int16_t input);

#endif

/* [] END OF FILE */
