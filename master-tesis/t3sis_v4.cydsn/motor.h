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

#define SPD_MEASUREMENT_FREQ_CLOCK 5000

#define _MOTOR_MIN_SPEED 0 // for pid iterm restart
#define _MOTOR_MAX_SPEED 8000

#define M_SPEED_CONTROL_MODE      0
#define M_POSITION_CONTROL_MODE   1
#define M_FORCE_CONTROL_MODE      2

#define M_DISABLE   0
#define M_ENABLE    1

#define M_CCW   0
#define M_CW    1
#define M_STOP  -1
    
#define M_PLOT_DATA_CMD 1

#define _MOTOR_MAX_POS 5000

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

    struct PID_t rvt_controller; // motor revolution controller
    struct PID_t spd_controller; // motor revolution controller
    
    PIN_t DIR;
    PIN_t ENABLE;

    uint8_t idx;    // indice del motor
    uint8_t control_mode; // 0: speed control, 1: position control, 2: tension control

    float spd_pid[3];
    int16_t rvt_pid_int[3];
    float rvt_pid[3];

    int16_t period_ha, rvt_aux, rvt_last_count, ca, ma, diff; // for speed and revolutions measurements

    int8_t curr_dir; // current rotor direction
    int16_t init_pos; // rotor init position

    int16_t curr_rvt;
    int16_t curr_spd;

    int16_t ref_rvt;
    int16_t ref_spd;

    int16_t rvtPID_out;
    int16_t spdPID_out;
    
    int16 last_count;

    /*
    int32_t readings[NUMREADINGS];      // the readings from the analog input
    int32_t readIndex;              // the index of the current reading
    int32_t total;                  // the running total
    int32_t average;                // the average
    */
    char dbg[DBG_SIZE];


} MOTOR_t;

void MOTOR_init(MOTOR_t* motor, PIN_t pin_enable, PIN_t pin_dir);
void MOTOR_initControlParams(MOTOR_t* motor, float* rvt);

void MOTOR_setSpeed(MOTOR_t* motor, int32_t ref);
void MOTOR_setPosition(MOTOR_t* motor);
void MOTOR_setRvtControlParams(MOTOR_t* motor, float kp, float ki, float kd);
void MOTOR_setSpdControlParams(MOTOR_t* motor, float kp, float ki, float kd);

/* debug level */
void motor_echod(int data);
void motor_echof(float data);

uint32 MOTOR_getSpdCounter(MOTOR_t* motor);
int16 MOTOR_getRvtCounter(MOTOR_t* motor);
void MOTOR_resetSpdCounter(MOTOR_t* motor);
void MOTOR_readSpeed(MOTOR_t* motor);

void MOTOR_setControlMode(MOTOR_t* motor, uint8_t mode);

void MOTOR_updateRevolution(MOTOR_t* motor);
void MOTOR_setRvtRef(MOTOR_t* motor, int16_t ref_rvt);
void MOTOR_setSpdRef(MOTOR_t* motor, int32_t ref);

// pin operations
void MOTOR_setPinDIR(MOTOR_t * motor, uint8_t setPin);
void MOTOR_setPinENABLE(MOTOR_t * motor, uint8_t setPin);

void MOTOR_setCCW(MOTOR_t * motor);
void MOTOR_setCW(MOTOR_t * motor);

void MOTOR_setEnable(MOTOR_t *motor);
void MOTOR_setDisable(MOTOR_t *motor);

int32_t MOTOR_getSpd(MOTOR_t* motor);
int8_t MOTOR_getDir(MOTOR_t* motor);
float MOTOR_getTR(MOTOR_t* motor, float alpha);
int32 MOTOR_get_tension_g(MOTOR_t* motor, int16 tension);

#endif
/* [] END OF FILE */
