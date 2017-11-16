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
  
extern MOTOR_t PM1,PM2;

CY_ISR_PROTO(PM1_HA_INT);          // process PM1_HA_ISR interrupt
CY_ISR_PROTO(PM2_HA_INT);          // process PM2_HA_ISR interrupt

CY_ISR_PROTO(READ_RVT_INT);          // process PM1_COUNT_ISR interrupt
CY_ISR_PROTO(CHECK_MOVEMENT_INT);  

CY_ISR_PROTO(SPD_COMMAND_INT);
CY_ISR_PROTO(RVT_COMMAND_INT);
CY_ISR_PROTO(TNS_COMMAND_INT);

#endif // ISR_FUNCTIONS_H

/* [] END OF FILE */
