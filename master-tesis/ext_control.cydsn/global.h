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

#define VEL_MAX 4000 // max vel for speed control
#define HF_CLK 10000 // rate of high frequency clock for speed measurement

#define MAX_COUNTER 6500 
    
//#define MANUAL_CONTROL
//#define NOT_MOVE_MOTOR

/* position control parameters */
#define KP_POS 0.3
#define KI_POS 0.0
#define KD_POS 0.0

/* speed control parameters */
#define KP_VEL 1.5
#define KI_VEL 0.2
#define KD_VEL 0.0

/* tensor control parameters */
#define KP_TENS 1.0
#define KI_TENS 0.0
#define KD_TENS 0.0

/* Project variables */
int _state_;

// for rotor speed and direction
extern volatile int16_t dir_count;
extern volatile int8_t rotor_direction;

// for speed measurement
extern volatile int32_t t_ha,ca,ma;

extern volatile uint8 _pVal;
extern volatile int16_t _tVal;
extern volatile int32_t speed_ref;
extern volatile int32_t reference_pos;
extern volatile int32_t pos_ref;
extern volatile int32_t current_speed;
extern volatile int32_t current_pos;

extern volatile int32_t debug;
extern volatile int32_t HA_counter;

extern volatile uint8 dir_state;
extern PID_t  speed_pid_;
extern PID_t  pos_pid_;
extern PID_t  tens_pid_;
extern uint8 _pid_pwm_out;

extern volatile uint8 Turn_serial,Turn,TurnLeft,TurnRight;

/* [] END OF FILE */
