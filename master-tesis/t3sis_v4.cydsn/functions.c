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

#include <functions.h>
#include <stdlib.h>

void sendPIDdata(int id)
{
    /*
    if (motors[id]->control_mode == 0)
        sprintf(strMsg,"*%d*%d*%d\r\n",motors[id]->spd_controller.kP,motors[id]->spd_controller.kI,motors[id]->spd_controller.kD);
    else if (motors[id]->control_mode == 1)
        sprintf(strMsg,"*%d*%d*%d\r\n", (int)(motors[id]->rvt_controller.kP*factor),(int)(motors[id]->rvt_controller.kI*factor),(int)(motors[id]->rvt_controller.kD*factor));
    else if (motors[id]->control_mode == 2)
        sprintf(strMsg,"*%d*%d*%d\r\n", (int)((float)motors[id]->rvt_controller.kP*factor),(int)((float)motors[id]->rvt_controller.kI*factor),(int)((float)motors[id]->rvt_controller.kD*factor));
    
    UART_PutString(strMsg);
    */
    WB.xff = 0xff;
    WB.cmd = 5; //send pid data
    if (motors[id]->control_mode == 0)
    {
        WB.ref = (int16_t)motors[id]->spd_controller.kP;
        WB.cur = (int16_t)motors[id]->spd_controller.kI;
        WB.val = (int16_t)motors[id]->spd_controller.kD;
    }
    else if (motors[id]->control_mode == 1)
    {
        WB.ref = (int16_t)motors[id]->rvt_controller.kP;
        WB.cur = (int16_t)motors[id]->rvt_controller.kI;
        WB.val = (int16_t)motors[id]->rvt_controller.kD;    
    }
    else if (motors[id]->control_mode == 2)
    {
        
    }
    
    // send pid data
    UART_PutArray((const uint8*)&WB.buffStr,COM_MSG_SIZE);
    
    
}

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{    
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (int32_t)(out_min + slope*(x - in_min));   
}

uint8_t fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, uint8_t out_min, uint8_t out_max)
{
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (out_min + slope*(x - in_min));
}

void ProcessCommandMsg(void)
{
    float p,i,d;
    char numaimp[6];
    switch(RB.cmd)
    {
        case 0: /* set reference of speed controller of motor RB.id */
            motors[RB.id]->ref_spd = RB.pref;
            break;
        case 1: /* set reference of position controller of motor RB.id */
            motors[RB.id]->ref_rvt = RB.pref;
            break;
        case 2: /* set reference of tension controller of motor RB.id */
        
            break;
        case 20: /* Stop data streaming */
            ContinuouslySendData = FALSE;
            break;
        case 22: /* request pid values */
            // cmd(0,12,-1,0,0,0,0) ask for position control PID values
            sendPIDdata(RB.id); // send controller parameters
            break;
        case 23: /* set control mode */
            // cmd(0,23,0,0,0,0) speed
            // cmd(0,23,1,0,0,0) position
            MOTOR_setControlMode(motors[RB.id],RB.pref);  
            break;
        case 24: /* stop motor */
            if(motors[RB.id]->ENABLE.STATE == 1){
                MOTOR_setPinENABLE(motors[RB.id], 0);
            }
            break;
        case 25: /* Set pid values */ 
            
            p = ((float)RB.P)/FLOAT_TO_INT_SCALE;
            i = ((float)RB.I)/FLOAT_TO_INT_SCALE;
            d = ((float)RB.D)/FLOAT_TO_INT_SCALE;
            
            echof(p);
            
            if(motors[RB.id]->control_mode == 0)
            {
                /*
                EEPROM_1_Write((const uint8*)&RB.P,(uint8)CYDEV_EE_BASE);
                EEPROM_1_Write((const uint8*)&RB.I,(uint8)CYDEV_EE_BASE+0x10);
                EEPROM_1_Write((const uint8*)&RB.D,(uint8)CYDEV_EE_BASE+0x20);
                */
                MOTOR_setSpdControlParams(motors[RB.id],p,i,d);
            }
            else if(motors[RB.id]->control_mode == 1)
            {
                /*
                EEPROM_1_Write((const uint8*)&p,3);
                EEPROM_1_Write((const uint8*)&i,4);
                EEPROM_1_Write((const uint8*)&d,5);
                */
                MOTOR_setRvtControlParams(motors[RB.id],p,i,d);   
            }
            
            break;
        case 40: /* Contuously send data */
            ContinuouslySendData = TRUE;
            break;
        case 44: /* resume motor */
            if(motors[RB.id]->ENABLE.STATE == 0){
                MOTOR_setPinENABLE(motors[RB.id], 1);
            }
            break;
        case 55: /* debug variables */
            sprintf(strMsg,"%d\t%d",(int)motors[0]->rvt_controller.inputValue,(int)(motors[0]->rvt_controller.kP*factor));
            UART_PutString(strMsg); 
            break;
        case 99: /* sw reset */
            CySoftwareReset();
            break;         
    }
    
}

/* [] END OF FILE */
