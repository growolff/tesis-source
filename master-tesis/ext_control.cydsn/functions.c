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

#include <functions.h>
#include <math.h>

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max){
    
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (int32_t)(out_min + slope*(x - in_min));
    
}

uint8 fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max){
    
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (uint8)(out_min + slope*(x - in_min));
    
}

double Sigmoid(double x,double a, double b)
{
    return a*(2/(1 + exp(-x*b))-1);
}


/* [] END OF FILE */
