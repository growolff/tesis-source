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

#define NUM_SENSORS 6

extern MOTOR_t PM1,PM2;
    
char msg[20];
int16 * TS_array;

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
uint8 fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

double Sigmoid(double x,double a, double b);

void ProcessCommandMsg(void);

void ToggleHandBrake(void);

int16 * StoreResults();

#endif // FUNCTIONS_H

/* [] END OF FILE */
