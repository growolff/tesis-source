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
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "global.h"
#include <project.h>
#include <stdint.h>
#include "math.h"


extern MOTOR_t PM1,PM2;
    
char msg[16];

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
uint8 fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

int32 get_tension_g(int16 tension);
float get_tension_kg(int16 tension);

double Sigmoid(double x,double a, double b);

void ProcessCommandMsg(void);

void ToggleHandBrake(void);

#endif // FUNCTIONS_H

/* [] END OF FILE */
