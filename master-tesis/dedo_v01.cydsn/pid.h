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

typedef struct
{
    int32_t kP;
    int32_t kI;
    int32_t kD;
    int32_t refVal;
    int32_t inputValue;
    int32_t lastValue;
    int32_t iTerm;
    int32_t minValue;
    int32_t maxValue;
    int32_t scaledMin;
    int32_t scaledMax;
} PID_t;


void PID_init(PID_t* pid, float p, float i, float d);
void PID_setCoeffs(PID_t* pid, float p, float i, float d);
void PID_setRef(PID_t* pid, int32_t pnt);
void PID_setMinValue(PID_t* pid, int32_t mn);
void PID_setMaxValue(PID_t* pid, int32_t mx);
int32_t PID_calculatePID(PID_t* pid, int32_t input);

#endif

/* [] END OF FILE */
