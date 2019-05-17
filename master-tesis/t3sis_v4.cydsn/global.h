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

// main loop check rate
#define RATE_HZ 40

//#define MAX_COUNTER 6500 // max counter for checking zero speed

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
extern PID_t  pos_pid_;

MOTOR_t PM1;
MOTOR_t PM2;

MOTOR_t* motors[2];

char strMsg[10];

float factor;

int pid_rvt[3];
int pid_spd[3];

char num[3]; // para printear
char lect[3]; // para leer de la eeprom
cystatus writeStatus;

char TransmitBuffer[TRANSMIT_BUFFER_SIZE];

void echo(char* data);
void echod(int data);
void echof(float data);

#endif // GLOBAL_VARIABLES_FILE_H

/* [] END OF FILE */