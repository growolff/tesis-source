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

/* Project defines */
#define FALSE  0
#define TRUE   1
#define TRANSMIT_BUFFER_SIZE  16

#define VEL_MAX 3000 // max vel for speed control
#define HF_CLK 10000 // rate of high frequency clock for speed measurement

#define MAX_COUNTER 6500 
    
#define MANUAL_CONTROL

/* position control parameters */
#define KP_POS 1.0
#define KI_POS 0.0
#define KD_POS 0.0

/* speed control parameters */
#define KP_VEL 0.6
#define KI_VEL 0.1
#define KD_VEL 0.0

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
extern volatile int32_t pos_ref;
extern volatile int32_t current_speed;
extern volatile int32_t current_pos;

extern volatile int32_t debug_pos;
extern volatile int32_t HA_counter;

extern volatile uint8 dir_state;
extern PID_t  speed_pid_;
extern PID_t  pos_pid_;

extern volatile uint8 Turn_serial,Turn,TurnLeft,TurnRight;

/* [] END OF FILE */
