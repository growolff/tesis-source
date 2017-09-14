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

#ifndef GLOBAL_VARIABLES_FILE_H
#define GLOBAL_VARIABLES_FILE_H

#include "stdio.h"
#include "project.h"
#include "pid.h"
#include "math.h"
#include "string.h"
#include "myUART.h"

/* Project structures */

//buffer to hold application settings
typedef struct TParamBuffer{
    char* cmd;  //command
    int pVal,iVal,dVal;   //int param
} ParamBuffer;  //settings
volatile ParamBuffer PB;     //volatile struct TParamBuffer PB;


/* Project defines */
#define FALSE  0
#define TRUE   1
#define TRANSMIT_BUFFER_SIZE  16

#define VEL_MAX 4000 // max vel for speed control
#define K_VEL 0.5 // constant for sigmoid in speed control

#define PWM_MAX 255 // max value for force control
#define K_PWM 0.02 // constan for sigmoidal function in force control

#define HF_CLK 10000 // rate of high frequency clock for speed measurement
#define MAX_COUNTER 6500 // max counter for checking zero speed

#define MAX_POSITION_LIMIT 72
#define MIN_POSITION_LIMIT 0

#define CONTROL_TYPE 1

#if CONTROL_TYPE==0
    #define MANUAL_CONTROL
#elif CONTROL_TYPE==1
    #define ANGLE_CONTROL
#elif CONTROL_TYPE==2
    #define SPEED_CONTROL
#elif CONTROL_TYPE==3
    #define TENDON_TENSION_CONTROL
#endif

/* position control parameters */
#define KP_POS 0.3
#define KI_POS 0.0
#define KD_POS 0.0

/* speed control parameters */
#define KP_VEL 2.0
#define KI_VEL 0.3
#define KD_VEL 0.0

/* tensor control parameters */
#define KP_TENS 0.5
#define KI_TENS 0.5
#define KD_TENS 0.5


/* Variable to store UART received character */
volatile uint8 Ch;
/* Flags used to store transmit data commands */
volatile uint8 ContinuouslySendData;
volatile uint8 SendSingleByte;
volatile uint8 SendEmulatedData;
volatile uint8 _uart_Reset;

/* Project variables */
int _state_;

// for rotor speed and direction
extern volatile int16_t dir_count;
extern volatile uint8 dir_state;
extern volatile int8_t rotor_direction;

// for speed measurement
extern volatile int32_t t_ha,ca,ma;

//for tension control
extern PID_t  tens_pid_;
extern volatile int32_t actual_tension;
extern volatile int32_t tension_ref;
extern volatile int16_t _tVal;

/* for position control */
extern PID_t  pos_pid_;
extern volatile int32_t actual_pos;
extern volatile int32_t pos_ref;
extern volatile int32_t init_pos;

/* for speed control */
extern PID_t  speed_pid_;
extern volatile int32_t actual_speed;
extern volatile int32_t speed_ref;

extern volatile uint8 _pVal;
extern volatile int32_t debug;

extern uint8 _pid_out;

#endif // GLOBAL_VARIABLES_FILE_H

/* [] END OF FILE */
