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

void echo(char* data)
{
    sprintf(strMsg,"%s\r\n", data);
    UART_PutString(strMsg);   
}
void echod(int data)
{
    sprintf(strMsg,"%d\r\n", data);
    UART_PutString(strMsg);   
}

void echof(float data)
{
    sprintf(strMsg,"%.2f\r\n", data);
    UART_PutString(strMsg);  
}

void sendPIDdata(int id)
{
    if (motors[id]->control_mode == 1)
        sprintf(strMsg,"*%d*%d*%d\r\n", (int)((float)motors[id]->rvt_controller.kP*1000.0),(int)((float)motors[id]->rvt_controller.kI*1000.0),(int)((float)motors[id]->rvt_controller.kD*1000));
    else if (motors[id]->control_mode == 2)
        sprintf(strMsg,"*%d*%d*%d\r\n", (int)((float)motors[id]->rvt_controller.kP*1000.0),(int)((float)motors[id]->rvt_controller.kI*1000.0),(int)((float)motors[id]->rvt_controller.kD*1000));
    else
        sprintf(strMsg,"*%d*%d*%d\r\n", (int)((float)motors[id]->rvt_controller.kP*1000.0),(int)((float)motors[id]->rvt_controller.kI*1000.0),(int)((float)motors[id]->rvt_controller.kD*1000));
        
    UART_PutString(strMsg);
}

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{    
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (int32_t)(out_min + slope*(x - in_min));   
}

uint8_t fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (uint8)(out_min + slope*(x - in_min));
}

void ProcessCommandMsg(void)
{
    //echo("TEST");
    //echo(RB.RxStr);
    //echod(RB.id);
    //echod(RB.cmd);
    //echod(RB.pref);
    //echod(RB.tref);
    float p,i,d;
    
    switch(RB.cmd)
    {
        case 1: /* set reference of position controller of motor RB.id */
            PM1.ref_rvt = RB.pref;
            break;
        case 2: /* set reference of tension controller of motor RB.id */
        
            break;
        case 12: /* request pid values */
            // cmd(0,12,-1,0,0,0,0) ask for position control PID values
            sendPIDdata(RB.id); // send controller parameters
            break;
        case 13: /* set control mode */
            // cmd(0,13,-1,0,0,0,0)
            if (RB.pref == -1){
                MOTOR_setControlMode(motors[RB.id],1);  
            } 
            else if (RB.tref == -1){
                MOTOR_setControlMode(motors[RB.id],2);
            }
            break;
        case 14: /* Set pid values */ 
            p = RB.P/1000.0;
            i = RB.I/1000.0;
            d = RB.D/1000.0;
            
            MOTOR_setRvtControlParams(motors[RB.id],p,i,d);
            /*
            if( motors[RB.id]->control_mode == 1){
                MOTOR_setRvtControlParams(motors[RB.id],(float)p,(float)i,(float)d);
            }*/
            
            //echo("Debug1");
            //echod(p*1000);
            //echo("Debug2");
            //echof(motors[RB.id]->rvt_controller.kP);    
            break;
        case 16:
            echod(RB.P);
            break;
        case 20: /* Stop data streaming */
            ContinuouslySendData = FALSE;
            break;
        case 24: /* stop motor */
            if(motors[RB.id]->ENABLE.STATE == 1)
                MOTOR_setPinENABLE(motors[RB.id], 0);
            break;
            
        case 40: /* Contuously send data */
            ContinuouslySendData = TRUE;
            break;
            
        case 44: /* resume motor */
            if(motors[RB.id]->ENABLE.STATE == 0)
                MOTOR_setPinENABLE(motors[RB.id], 1);
            break;
        case 99: /* sw rebbot */
            CySoftwareReset();
            break;
                
    }
    
}

/* [] END OF FILE */
