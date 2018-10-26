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

#include "isr_functions.h"

CY_ISR(PM1_HA_INT) //interrupt on level change in PM1_HA signal
{
    MOTOR_readCurrentSpeed(&PM1,1);
}

CY_ISR(RVT_COMMAND_INT)
{
    MOTOR_readCurrentRevolution(&PM1,1);
    MOTOR_setRvtRef(&PM1,PM1.ref_rvt);
    if(PM1.control_mode == 1){
        PM1.rvtPID_out < 0 ? MOTOR_setPinDIR(&PM1,1) : MOTOR_clearPinDIR(&PM1);
        PM1.ref_spd = fn_mapper((int32_t)fabs((float)PM1.rvtPID_out),0,100,0,VEL_MAX);
    }
   
}

/* [] END OF FILE */
