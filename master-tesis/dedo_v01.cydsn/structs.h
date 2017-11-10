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

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "motor.h"

typedef struct FINGER_t
{
    MOTOR_t MF,ME;
    
    
} FINGER_t;


typedef struct HAND_t
{
    FINGER_t I,M,P;
    
    
} HAND_t;

    
    
#endif //STRUCTS_H_

/* [] END OF FILE */
