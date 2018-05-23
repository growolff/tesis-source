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

#define OPEN_EXTENSOR_POS_REF 20
#define OPEN_FLECTOR_POS_REF 0
#define CLOSE_EXTENSOR_POS_REF 0
#define CLOSE_FLECTOR_POS_REF 20
    
#define ANTAGONIST 0

#include "motor.h"
#include "global.h"

typedef struct FINGER_t
{
    MOTOR_t MF,ME;
   
    int32_t curr_tns;
    int32_t ref_tns;
    int32_t tnsPID_out;
    
    uint8 control_mode;
    
} FINGER_t;

void FINGER_initFinger(FINGER_t* finger);
void FINGER_command(FINGER_t* finger, uint8 command);

void FINGER_setTnsRef(MOTOR_t* motor, int32_t tnsRef);
void FINGER_readCurrentTension(MOTOR_t* motor, uint8 motor_number);
void FINGER_setTnsControlParams(MOTOR_t* motor, float kp, float ki, float kd);






typedef struct HAND_t
{
    FINGER_t I,M,P;
    
    
} HAND_t;

    
    
#endif //STRUCTS_H_

/* [] END OF FILE */
