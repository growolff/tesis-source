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

#include "global.h"
#include <project.h>
#include <stdint.h>
#include "math.h"

/* list of commands */
#define F_SEND_DATA_TRUE      40
#define F_SEND_DATA_FALSE     20
#define F_SET_POS_REF         1
#define F_SET_FORCE_REF       2
#define F_REQ_PID_VALUES      22
#define F_SET_PID_VALUES      23
#define F_SET_CONTROL_MODE    24
#define F_DISABLE_MOTOR       25
#define F_ENABLE_MOTOR        26
#define F_DEBUG_VAR           55
#define F_SOFTWARE_RESET      99    
    
extern MOTOR_t PM1;

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
uint8_t fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, uint8_t out_min, uint8_t out_max);

void ProcessCommandMsg(void);

#endif // FUNCTIONS_H

/* [] END OF FILE */
