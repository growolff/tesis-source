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

CY_ISR(READ_RVT_INT)
{   
    MOTOR_readCurrentRevolution(&PM1,1);
    MOTOR_readCurrentRevolution(&PM2,2);
    //MOTOR_readCurrentTension(&PM1,1);
}

CY_ISR(CHECK_MOVEMENT_INT)
{   
    MOTOR_checkDir(&PM1,1);
    MOTOR_checkDir(&PM2,2);
}

CY_ISR(SPD_COMMAND_INT)
{
    // Write speed command in motor PM1 VDAC
    MOTOR_setSpdRef(&PM1,PM1.ref_spd);
    if(PM1.control_mode == 1 || PM1.control_mode == 2){
        PM1_SPD_VDAC8_SetValue((uint8)(fn_mapper_8b(PM1.spdPID_out,0,9500,0,255)));
        //MOTOR_commandDriver(&PM1,PMF,(uint8)(fn_mapper_8b(PM1.spdPID_out,0,10000,0,255)));
    }
    
    // Write speed command in motor PM2 VDAC
    MOTOR_setSpdRef(&PM2,PM2.ref_spd);
    if(PM2.control_mode == 1 || PM2.control_mode == 2){
        PM2_SPD_VDAC8_SetValue((uint8)(fn_mapper_8b(PM2.spdPID_out,0,9500,0,255)));
        //MOTOR_commandDriver(&PM2,PME,fn_mapper_8b(PM2.spdPID_out,0,10000,0,255));
    }
}

CY_ISR(RVT_COMMAND_INT)
{
    MOTOR_setRvtRef(&PM1,PM1.ref_rvt);
    if(PM1.control_mode == 1){
        PM1.rvtPID_out < 0 ? MOTOR_setPinDIR(&PM1) : MOTOR_clearPinDIR(&PM1);
        PM1.ref_spd = fn_mapper((int32_t)fabs((float)PM1.rvtPID_out),0,100,0,VEL_MAX);
    }

    MOTOR_setRvtRef(&PM2,PM2.ref_rvt);
    if(PM2.control_mode == 1){
        PM2.rvtPID_out < 0 ? MOTOR_setPinDIR(&PM2) : MOTOR_clearPinDIR(&PM2);
        PM2.ref_spd = fn_mapper((int32_t)fabs((float)PM2.rvtPID_out),0,100,0,VEL_MAX);
    }
}

CY_ISR(TNS_COMMAND_INT)
{
    MOTOR_setTnsRef(&PM1,PM1.ref_tns);
    PM1.tension_control_signal = (float)PM1.tnsPID_out/MOTOR_getTR(&PM1,(float)PM1.curr_rvt/4.0);
    if(PM1.control_mode == 3){
        PM1.tnsPID_out < 0 ? MOTOR_setPinDIR(&PM1) : MOTOR_clearPinDIR(&PM1);
        MOTOR_commandDriver(&PM1,PMF,fn_mapper_8b(fabs(PM1.tension_control_signal),0,125,0,255));
    }
    MOTOR_setTnsRef(&PM2,PM2.ref_tns);
    PM2.tension_control_signal = (float)PM2.tnsPID_out/MOTOR_getTR(&PM2,(float)PM2.curr_rvt/4.0);
    if(PM2.control_mode == 3){
        PM2.tnsPID_out < 0 ? MOTOR_setPinDIR(&PM2) : MOTOR_clearPinDIR(&PM2);
        MOTOR_commandDriver(&PM2,PME,fn_mapper_8b(fabs(PM2.tension_control_signal),0,125,0,255));
    }
}

/* [] END OF FILE */
