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

#include "stdio.h"
#include "project.h"
#include "functions.h"
#include "pid.h"
#include "math.h"

/* Project defines */
#define FALSE  0
#define TRUE   1
#define TRANSMIT_BUFFER_SIZE  16

#define VEL_MAX 3000 // max vel for speed control
#define HF_CLK 10000 // rate of high frequency clock for speed measurement

#define MAX_COUNTER 6500 
    
//#define MANUAL_CONTROL
#define T_CONTROL

#ifndef T_CONTROL
#define P_CONTROL
#endif

/* position control parameters */
#define KP_POS 0.23
#define KI_POS 0.0
#define KD_POS 0.0

/* speed control parameters */
#define KP_VEL 1.5
#define KI_VEL 0.2
#define KD_VEL 0.0

/* tensor control parameters */
#define KP_TENS 0.2
#define KI_TENS 0.0
#define KD_TENS 0.0

/* Project variables */
int _state_;

// for rotor speed and direction
extern volatile int16_t dir_count;
extern volatile int8_t rotor_direction;

// for speed measurement
extern volatile int32_t t_ha,ca,ma;

//for tension control
extern PID_t  tens_pid_;
extern volatile int32_t actual_tension;
extern volatile int32_t ref_tension;
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
extern volatile uint8 dir_state;

extern uint8 _pid_pwm_out;

extern volatile uint8 Turn_serial,Turn,TurnLeft,TurnRight;

/* [] END OF FILE */
