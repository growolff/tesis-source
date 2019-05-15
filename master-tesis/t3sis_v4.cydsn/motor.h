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

#include "pid_o.h"
#include "project.h"
#include "math.h"
#include <stdio.h>

#define PMF 1
    
#define HIGH_FREQ_CLOCK 100000
    
#define DBG_SIZE 10
   
#define NUM_SENSORS 3
int16 * TS_array;
    
char msg[10];
    
#define NUMREADINGS 50
    
typedef struct PIN_t
{
    reg8 *DR;
    uint8 MASK;
    uint8 STATE;
} PIN_t;
    
typedef struct MOTOR_t {
    uint8_t control_mode; // 0: speed contron, 1: position control, 2: tension control

    struct PID_t rvt_controller; // motor revolution controller
    struct PID_t spd_controller; // motor revolution controller
    
    float rvt_params[3];
    float spd_params[3];
    
    int32_t period_ha, rvt_aux, rvt_last_count, ca, ma, diff; // for speed and revolutions measurements
    
    int8_t curr_dir; // current rotor direction
    int32_t init_pos; // rotor init position
    
    int32_t curr_rvt;
    int32_t curr_spd;
    
    int32_t ref_rvt;
    int32_t ref_spd;
    
    int32_t rvtPID_out;
    int32_t spdPID_out;
    
    PIN_t DIR;
    PIN_t ENABLE;
         
    int32_t readings[NUMREADINGS];      // the readings from the analog input
    int32_t readIndex;              // the index of the current reading
    int32_t total;                  // the running total
    int32_t average;                // the average
   
    char dbg[DBG_SIZE];
    
    
} MOTOR_t;

void MOTOR_init(MOTOR_t* motor, PIN_t pin_enable, PIN_t pin_dir);
void MOTOR_initControlParams(MOTOR_t* motor, float* rvt);
void MOTOR_setRvtControlParams(MOTOR_t* motor, float kp, float ki, float kd);

void MOTOR_setSpdControlParams(MOTOR_t* motor, float kp, float ki, float kd);

/* debug level */
void motor_echod(int data);
void motor_echof(float data);
void dbgLed();

void MOTOR_readSpeed(MOTOR_t* motor);
//void MOTOR_readSpeed_2(MOTOR_t* motor);
void MOTOR_readRevolution(MOTOR_t* motor);

void MOTOR_setControlMode(MOTOR_t* motor, uint8_t mode);

void MOTOR_checkDir(MOTOR_t* motor);

void MOTOR_setRvtRef(MOTOR_t* motor);
void MOTOR_setSpdRef(MOTOR_t* motor, int32_t pnt);

float MOTOR_getTR(MOTOR_t* motor, float alpha);
int32 MOTOR_get_tension_g(MOTOR_t* motor, int16 tension);

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
