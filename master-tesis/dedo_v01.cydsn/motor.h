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

#ifndef MOTOR_H_
#define MOTOR_H_    

#include "pid.h"
#include "project.h"
    
typedef struct MOTOR_t {
    uint8_t control_mode;

    PID_t rvt_controller; // motor revolution
    PID_t spd_controller; // motor speed
    PID_t tns_controller; // string tension
    
    float spd_params[3];
    float rvt_params[3];
    float tns_params[3];
    
    int32_t period_ha;
    int8_t curr_dir; // current rotor direction
    
    int32_t curr_rvt;
    int32_t curr_spd;
    int32_t curr_tns;
    
    int32_t ref_rvt;
    int32_t ref_spd;
    int32_t ref_tns;
    
    int32_t spdOutPID;
    
    int32_t rvt_aux, rvt_last_count;

    uint8_t ENABLE;
    uint8_t BRAKEn;
    uint8_t DIR;

} MOTOR_t;

void MOTOR_init(MOTOR_t* motor);
void MOTOR_setMode(MOTOR_t* motor, uint8_t mode);
void MOTOR_setControlParams(MOTOR_t* motor, float* rvt, float* spd, float* tns);
void MOTOR_setRvtRef(MOTOR_t* motor, int32_t rvtRef);
void MOTOR_setSpdRef(MOTOR_t* motor, int32_t spdRef);
void MOTOR_setTnsRef(MOTOR_t* motor, int32_t tnsRef);

void MOTOR_ToggleHandBrake(MOTOR_t* motor);

int32_t MOTOR_getSpd(MOTOR_t* motor);
int8_t MOTOR_getDir(MOTOR_t* motor);
    
    
#endif
/* [] END OF FILE */
