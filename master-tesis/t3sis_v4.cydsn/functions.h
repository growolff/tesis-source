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
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <project.h>
#include <stdint.h>
#include "global.h"
#include "myUART.h"
#include "math.h"

/* list of commands */
#define F_SEND_DATA_TRUE      40
#define F_SEND_DATA_FALSE     20
#define F_SET_POS_REF         1
#define F_SET_FORCE_REF       2
#define F_GET_PID_VALUES      21
#define F_REQ_PID_VALUES      22
#define F_SET_PID_VALUES      23
#define F_SET_CONTROL_MODE    24
#define F_DISABLE_MOTOR       25
#define F_ENABLE_MOTOR        26
#define F_DEBUG_VAR           55
#define F_SOFTWARE_RESET      99    
   
char strMsg[10];

void echo(char* data);
void echod(int data);
void echof(float data);
void echomsg(uint8_t cmd, int16_t ref, int16_t cur, int16_t val);
void sendPIDdata(int id);

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
uint8_t fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, uint8_t out_min, uint8_t out_max);

// sensor conditioning functions
void read_smooth(int numRead);
uint16_t getTension(uint16_t read);

void ProcessCommandMsg(void);

#endif // FUNCTIONS_H

/* [] END OF FILE */
