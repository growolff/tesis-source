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

typedef struct PIN_t
{
    reg8 *DR;
    uint8 MASK;
    uint8 STATE;
} PIN_t;
    
typedef struct MOTOR_t {
    uint8_t control_mode;

    PID_t rvt_controller; // motor revolution
    PID_t spd_controller; // motor speed
    PID_t tns_controller; // string tension
    
    float spd_params[3];
    float rvt_params[3];
    float tns_params[3];
    
    int32_t period_ha, rvt_aux, rvt_last_count, ca, ma; // for speed and revolutions measurements
    
    int8_t curr_dir; // current rotor direction
    int32_t init_pos; // rotor init position
    
    int32_t curr_rvt;
    int32_t curr_spd;
    int32_t curr_tns;
    
    int32_t ref_rvt;
    int32_t ref_spd;
    int32_t ref_tns;
    
    int32_t spdPID_out;
    int32_t rvtPID_out;
    int32_t tnsPID_out;

    PIN_t BRAKEn;
    PIN_t DIR;
    PIN_t ENABLE;

} MOTOR_t;

void MOTOR_init(MOTOR_t* motor, PIN_t pin_enable, PIN_t pin_braken, PIN_t pin_dir);
void MOTOR_setControlMode(MOTOR_t* motor, uint8_t mode);
void MOTOR_initControlParams(MOTOR_t* motor, float* rvt, float* spd, float* tns);
void MOTOR_setSpdControlParams(MOTOR_t* motor, float kp, float ki, float kd);
void MOTOR_setRvtControlParams(MOTOR_t* motor, float kp, float ki, float kd);
void MOTOR_resetVariables(MOTOR_t* motor);

void MOTOR_setRvtRef(MOTOR_t* motor, int32_t rvtRef);
void MOTOR_setSpdRef(MOTOR_t* motor, int32_t spdRef);
void MOTOR_setTnsRef(MOTOR_t* motor, int32_t tnsRef);

// pin operations
void MOTOR_ToggleHandBrake(MOTOR_t* motor);

void MOTOR_clearPinBRAKEn(MOTOR_t * motor);
void MOTOR_clearPinDIR(MOTOR_t * motor);
void MOTOR_clearPinENABLE(MOTOR_t * motor);
void MOTOR_setPinBRAKEn(MOTOR_t * motor);
void MOTOR_setPinDIR(MOTOR_t * motor);
void MOTOR_setPinENABLE(MOTOR_t * motor);

int32_t MOTOR_getSpd(MOTOR_t* motor);
int8_t MOTOR_getDir(MOTOR_t* motor);
    
    
#endif
/* [] END OF FILE */
