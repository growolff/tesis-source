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
#ifndef ISR_FUNCTIONS_H
#define ISR_FUNCTIONS_H

#include "global.h"

int32_t PM1_HA_ca,PM1_HA_ma;
int32_t last_count;
volatile int32_t PM1_HA_Period;
volatile int8_t rotor_direction;

CY_ISR_PROTO(PM1_HA_INT);          // process PM1_HA_ISR interrupt
CY_ISR_PROTO(COUNT_INT);          // process PM1_COUNT_ISR interrupt

#endif // ISR_FUNCTIONS_H

/* [] END OF FILE */
