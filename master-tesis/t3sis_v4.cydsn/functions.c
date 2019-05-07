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
    if (motors[id]->control_mode == 1)
        sprintf(strMsg,"*%d*%d*%d\r\n", (int)(motors[id]->rvt_controller.kP*factor),(int)(motors[id]->rvt_controller.kI*factor),(int)(motors[id]->rvt_controller.kD*factor));
    else if (motors[id]->control_mode == 2)
        sprintf(strMsg,"*%d*%d*%d\r\n", (int)((float)motors[id]->rvt_controller.kP*factor),(int)((float)motors[id]->rvt_controller.kI*factor),(int)((float)motors[id]->rvt_controller.kD*factor));
    else
        sprintf(strMsg,"*%d*%d*%d\r\n", (int)((float)motors[id]->rvt_controller.kP*factor),(int)((float)motors[id]->rvt_controller.kI*factor),(int)((float)motors[id]->rvt_controller.kD*factor));
        
    UART_PutString(strMsg);
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
    //echo("TEST");
    //echo(RB.RxStr);
    //echod(RB.id);
    //echod(RB.cmd);
    //echod(RB.pref);
    float p,i,d;
    
    switch(RB.cmd)
    {
        case 1: /* set reference of position controller of motor RB.id */
            PM1.ref_rvt = RB.pref;
            break;
        case 2: /* set reference of tension controller of motor RB.id */
        
            break;
        case 22: /* request pid values */
            // cmd(0,12,-1,0,0,0,0) ask for position control PID values
            sendPIDdata(RB.id); // send controller parameters
            break;
            
        case 23: /* set control mode */
            // cmd(0,13,1,0,0,0,0)
            //MOTOR_setControlMode(motors[RB.id],RB.pref);  
            echo("DBGctrl");
            break;
        case 25: /* Set pid values */ 
            p = RB.P/100.0;
            i = RB.I/100.0;
            d = RB.D/100.0;

            writeStatus = EEPROM_1_WriteByte((uint8_t)RB.P,0);
            writeStatus = EEPROM_1_WriteByte((uint8_t)RB.I,1);
            writeStatus = EEPROM_1_WriteByte((uint8_t)RB.D,2);
            
            MOTOR_setRvtControlParams(motors[RB.id],p,i,d);
            /*
            if( motors[RB.id]->control_mode == 1){
                MOTOR_setRvtControlParams(motors[RB.id],p,i,d);
                echo("Dbgpid2");
            }
            */
            
            //echod(p*1000);
            //echo("Debug2");
            //echof(motors[RB.id]->rvt_controller.kP);    
            break;
        case 16:

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
        case 55: /* debug variables */
            sprintf(strMsg,"%d\t%d",(int)motors[0]->rvt_controller.inputValue,(int)(motors[0]->rvt_controller.kP*factor));
            UART_PutString(strMsg); 
            break;
        case 99: /* sw rebbot */
            CySoftwareReset();
            break;
                
    }
    
}

/* [] END OF FILE */
