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
    PM1.ca = PM1_HA_TIMER_ReadCounter();
    PM1.period_ha = -1*((PM1.ca)+PM1.ma);
    PM1.ma = PM1.ca;
    PM1_HA_TIMER_WriteCounter(0);
    
    PM1.curr_spd = (HF_CLK/PM1.period_ha) * 30;
}

CY_ISR(COUNT_INT) //interrupt on level change in PM1_HA signal
{   
    PM1.rvt_aux = PM1_DirCounter_GetCounter();
    PM1.curr_rvt = PM1.rvt_aux - PM1.init_pos;
    if ( PM1.rvt_aux - PM1.rvt_last_count > 0 )
    {
        // if > 0 its turning right (CW) 
        PM1.curr_dir = 1;
    }
    else if ( PM1.rvt_aux - PM1.rvt_last_count < 0 )
    {
        // if < 0 its turning left (CCW)
        PM1.curr_dir = -1;
    }
    else if ( PM1.rvt_aux - PM1.rvt_last_count == 0 )
    {
        // if = 0 its not rotating
        PM1.curr_dir = 0;
        PM1.curr_spd = 0;
    }
    // update variable
    PM1.rvt_last_count = PM1.rvt_aux; 
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
}


/* [] END OF FILE */
