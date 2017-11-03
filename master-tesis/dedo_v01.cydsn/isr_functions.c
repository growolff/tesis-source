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
    PM1_HA_ca = PM1_HA_TIMER_ReadCounter();
    PM1_HA_Period = -1*((PM1_HA_ca)+PM1_HA_ma);
    PM1_HA_ma = PM1_HA_ca;
    PM1_HA_TIMER_WriteCounter(0);
    
    PM1.curr_spd = (HF_CLK/PM1_HA_Period) * 30;
}

CY_ISR(COUNT_INT) //interrupt on level change in PM1_HA signal
{   
    PM1.rvt_aux = PM1_DirCounter_GetCounter();
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
    #if defined(ANGLE_CONTROL) || defined(SPEED_CONTROL)
        PM1_SPD_VDAC8_SetValue((uint8)(fn_mapper_8b(PM1.spdOutPID,0,10000,0,255)));
    #endif   
    
}


/* [] END OF FILE */
