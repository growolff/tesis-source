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
#include "math.h"

#define PMF 1 
#define PME 2
#define MMF 3
#define MME 4
#define IMF 5
#define IME 6
    
#define HIGH_FREQ_CLOCK 20000
    
#define VEL_MAX_MOTOR 9000
#define NUM_SENSORS 6
int16 * TS_array;
    
#define NUMREADINGS 50
    
typedef struct PIN_t
{
    reg8 *DR;
    uint8 MASK;
    uint8 STATE;
} PIN_t;
    
typedef struct MOTOR_t {
    uint8_t control_mode;

    struct PID_t rvt_controller; // motor revolution
    PID_t tns_controller; // string tension
    
    float rvt_params[3];
    float tns_params[3];
    
    int32_t period_ha, rvt_aux, rvt_last_count, ca, ma; // for speed and revolutions measurements
    
    int8_t curr_dir; // current rotor direction
    int32_t init_pos; // rotor init position
    
    int32_t curr_rvt;
    int32_t curr_tns;
    int32_t curr_spd;
    
    int32_t ref_rvt;
    int32_t ref_tns;
    int32_t ref_spd;
    
    VAR_TYPE rvtPID_out;
    int32_t tnsPID_out; 

    PIN_t DIR;
    PIN_t ENABLE;
    
    // for TR calculus
    float R,L,A;
    float tension_control_signal;
    
    int32_t error_check_counter;
     
    int32_t readings[NUMREADINGS];      // the readings from the analog input
    int32_t readIndex;              // the index of the current reading
    int32_t total;                  // the running total
    int32_t average;                // the average
   
    
    
} MOTOR_t;

void MOTOR_init(MOTOR_t* motor, PIN_t pin_enable, PIN_t pin_dir);
void MOTOR_setControlMode(MOTOR_t* motor, uint8_t mode);
void MOTOR_initControlParams(MOTOR_t* motor, float* rvt, float* tns);
void MOTOR_setRvtControlParams(MOTOR_t* motor, float kp, float ki, float kd);
void MOTOR_setTnsControlParams(MOTOR_t* motor, float kp, float ki, float kd);
void MOTOR_resetVariables(MOTOR_t* motor);

void MOTOR_readSpeed(MOTOR_t* motor);
void MOTOR_readRevolution(MOTOR_t* motor);
void MOTOR_readTension(MOTOR_t* motor, uint8 motor_number);

void MOTOR_checkDir(MOTOR_t* motor, uint8 motor_number);
void MOTOR_fixParche(MOTOR_t* motor);
void MOTOR_checkError(MOTOR_t* motor);

void MOTOR_externControl(MOTOR_t* motor, uint8 ctrl, int32 ref);
void MOTOR_setRef(MOTOR_t* motor, uint8 ctrl, int32 ref);
void MOTOR_setRvtRef(MOTOR_t* motor);
void MOTOR_setSpdRef(MOTOR_t* motor, int32_t spdRef);
void MOTOR_setTnsRef(MOTOR_t* motor, int32_t tnsRef);

float MOTOR_getTR(MOTOR_t* motor, float alpha);
int32 MOTOR_get_tension_g(MOTOR_t* motor, int16 tension);

void MOTOR_commandDriver(MOTOR_t* motor, uint8 motor_number, uint8 speed_value);
void MOTOR_sendSpeedCommand(uint8 motor, uint8 speed_value);

uint8 MOTOR_checkActuatorLimits(MOTOR_t* motor);

int16 * MOTOR_StoreADCResults();

// pin operations
void MOTOR_ToggleDir(MOTOR_t* motor);

void MOTOR_clearPinDIR(MOTOR_t * motor);
void MOTOR_clearPinENABLE(MOTOR_t * motor);
void MOTOR_setPinDIR(MOTOR_t * motor, uint8_t setPin);
void MOTOR_setPinENABLE(MOTOR_t * motor, uint8_t setPin);

int32_t MOTOR_getSpd(MOTOR_t* motor);
int8_t MOTOR_getDir(MOTOR_t* motor);
    
    
#endif
/* [] END OF FILE */
