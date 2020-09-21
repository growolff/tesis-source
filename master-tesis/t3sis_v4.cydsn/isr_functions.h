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

extern MOTOR_t M1,M2;

CY_ISR_PROTO(M1_HA_INT);          // process PM1_HA_ISR interrupt
CY_ISR_PROTO(M2_HA_INT);          // process PM2_HA_ISR interrupt

CY_ISR_PROTO(SPD_M2_INT);

#endif // ISR_FUNCTIONS_H

/* [] END OF FILE */
