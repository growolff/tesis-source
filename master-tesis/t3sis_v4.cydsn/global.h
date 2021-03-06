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

#include "project.h"
#include "motor.h"
#include "stdio.h"
#include "pid_o.h"
#include "string.h"
#include "myUART.h"
#include "functions.h" 
#include "isr_functions.h"
#include "math.h"
#include "stdlib.h"
    
/* Project defines */
#define FALSE  0
#define TRUE   1
#define TRANSMIT_BUFFER_SIZE  16

// main loop control rates
#define LED_BLINK_RATE 2    
#define RATE_HZ 10
#define RVT_RATE_HZ 100

/* Variable to store UART received character */
uint8 Ch;
/* Flags used to store transmit data commands */
volatile uint8 ContinuouslySendData;
volatile uint8 SendSingleByte;
volatile uint8 SendEmulatedData;
volatile uint8 SoftwareReset;

/* Project variables */
int _state_;
   
// for rotor speed and direction
extern volatile uint8 dir_state;
extern volatile int8_t rotor_direction;

/* for position control */
PID_t  pos_pid_m1;
PID_t  pos_pid_m2;

#define M1_IDX 0
#define M2_IDX 1

/* SENSOR_ADC connection order*/
#define _FS1 2
#define _FS2 0
#define _POTE 1    
    
#define M1_KP 4.5
#define M1_KI 0.1
#define M1_KD 0.1
    
#define M2_KP 2.5
#define M2_KI 0.2
#define M2_KD 0.1
    
MOTOR_t M1;
MOTOR_t M2;
    
#define NUM_MOTORS 2

int16_t sumFs1;
int16_t sumFs2;
int16_t sumPote;
    
PID_t* pid[2];
MOTOR_t* motors[2];

float factor;

int pid_rvt[3];
int pid_spd[3];

char TransmitBuffer[TRANSMIT_BUFFER_SIZE];

void echo(char* data);
void echod(int data);
void echof(float data);

#endif // GLOBAL_VARIABLES_FILE_H

/* [] END OF FILE */


