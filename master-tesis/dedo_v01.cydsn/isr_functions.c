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
CY_ISR(PM2_HA_INT) //interrupt on level change in PM1_HA signal
{
    MOTOR_readCurrentSpeed(&PM2,2);
}

CY_ISR(COUNT_INT)
{   
    MOTOR_readCurrentRevolution(&PM1,1);
    MOTOR_readCurrentRevolution(&PM2,2);
}

CY_ISR(SPD_COMMAND_INT)
{
    // Write speed command in motor PM1 VDAC
    if( fabs((float)PM1.ref_spd - (float)PM1.curr_spd) > 0 ){
        MOTOR_setSpdRef(&PM1,PM1.ref_spd);

        if(PM1.control_mode == 1 || PM1.control_mode == 2){
            PM1_SPD_VDAC8_SetValue((uint8)(fn_mapper_8b(PM1.spdPID_out,0,10000,0,255)));
        }
    }
    // Write speed command in motor PM1 VDAC
    if( fabs((float)PM2.ref_spd - (float)PM2.curr_spd) > 0 ){
        MOTOR_setSpdRef(&PM2,PM2.ref_spd);

        if(PM2.control_mode == 1 || PM2.control_mode == 2){
            PM2_SPD_VDAC8_SetValue((uint8)(fn_mapper_8b(PM2.spdPID_out,0,10000,0,255)));
        }
    }
}

CY_ISR(RVT_COMMAND_INT)
{
    if( fabs((float)PM1.ref_rvt - (float)PM1.curr_rvt) > 0 ){
        MOTOR_setRvtRef(&PM1,PM1.ref_rvt);
        
        if(PM1.control_mode == 1){
            PM1.rvtPID_out < 0 ? MOTOR_setPinDIR(&PM1) : MOTOR_clearPinDIR(&PM1);
            PM1.ref_spd = (int)fabs(Sigmoid(PM1.rvtPID_out,VEL_MAX,K_VEL));
            MOTOR_setSpdRef(&PM1,PM1.ref_spd);
        }
    }
    if( fabs((float)PM2.ref_rvt - (float)PM2.curr_rvt) > 0 ){
        MOTOR_setRvtRef(&PM2,PM2.ref_rvt);
        
        if(PM2.control_mode == 1){
            PM2.rvtPID_out < 0 ? MOTOR_setPinDIR(&PM2) : MOTOR_clearPinDIR(&PM2);
            PM2.ref_spd = (int)fabs(Sigmoid(PM2.rvtPID_out,VEL_MAX,K_VEL));
            MOTOR_setSpdRef(&PM2,PM2.ref_spd);
        }
    }
}


/* [] END OF FILE */
