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

CY_ISR(M1_HA_INT) //interrupt on level change in PM1_HA signal
{
    MOTOR_readSpeed(motors[0]);
}
CY_ISR(M2_HA_INT) //interrupt on level change in PM1_HA signal
{
    //MOTOR_readSpeed(motors[1]);   
}
CY_ISR(SPD_M2_INT) //interrupt at 200hz signal
{ 
    int16 counter = MOTOR_getRvtCounter(motors[1]);
    if(counter >= 0 || counter <= 5000){
        motors[1]->curr_spd = (counter-ante)*60*20/10;
    }
    else{
        counter = ante;
    }    
    ante = counter;
}

/* [] END OF FILE */
