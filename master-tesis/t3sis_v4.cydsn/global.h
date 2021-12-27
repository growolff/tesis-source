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
#include "finger.h"
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
#define RATE_HZ 15
#define RVT_RATE_HZ 2000
#define TENSION_RATE_HZ 200

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

/* SENSOR_ADC connection order*/
#define _FS1 2
#define _FS2 0
#define _POTE 1  
int16_t sumFs1;
int16_t sumFs2;
int16_t sumPote;

// M1 position control parameters (finger 1)
#define M1_KP 2.4
#define M1_KI 0.0
#define M1_KD 0.0
// M2 position control parameters (finger 1)
#define M2_KP 2.4
#define M2_KI 0.0
#define M2_KD 0.0
// finger 1 tension control parameters    
#define F1_T_KP 1.0
#define F1_T_KI 0.1
#define F1_T_KD 1.0
    
    
#define NUM_MOTORS 3
#define NUM_FINGERS 2
    
MOTOR_t* motors[NUM_MOTORS];
MOTOR_t M1,M2;
MOTOR_t M3,M4;

FINGER_t indice;
FINGER_t pulgar;
FINGER_t* fingers[NUM_FINGERS];

#define SPD_CTRL 0
#define REV_CTRL 1
#define TEN_CTRL 2

PID_t* pid[2];
int pid_rvt[3];
int pid_spd[3];
int pid_ten[3];

float factor;

char TransmitBuffer[TRANSMIT_BUFFER_SIZE];

void echo(char* data);
void echod(int data);
void echof(float data);

#endif // GLOBAL_VARIABLES_FILE_H

/* [] END OF FILE */


