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

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{    
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (int32_t)(out_min + slope*(x - in_min));   
}

uint8 fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (uint8)(out_min + slope*(x - in_min));
}

int32 get_tension_g(int16 tension)
{
       return (int32)((5000.0/4096.0)*tension/0.0035)/1000;
}

void ProcessCommandMsg(void)
{    
    //check received message for any valid command and execute it if necessary or report old value
    //if command not recognized, then report error
    //todo: add check for valid conversion string->value
     
    //sprintf(strMsg1,"%s\r", RB.RxStr); UART_PutString(strMsg1);
   
    //todo: ther are problems if terminator is "\r\n"
    uint8 updatePID = FALSE;
    enum msg_byte {sof, read_write, motor, digital_pin, onoff, ctrl, value} current_state, next_state;
    current_state = sof;
    //uint8_t motor;
    //uint8_t ctrl;
    char ref[4];
    //char* motor,ctrl;
    switch(RB.cmd)
    {
        case '0':
            sprintf(msg,"&%c%c%s\r\n",RB.cmd,RB.dp,RB.valstr);
            UART_PutString(msg);
            MOTOR_clearPinBRAKEn(&PM1);
            MOTOR_clearPinDIR(&PM1);
            MOTOR_clearPinENABLE(&PM1);
            current_state = sof;
            
            switch(current_state)
            {
                case sof:
                    if(RB.sof == 'x')
                        next_state = read_write;
                    break;
                case read_write:
                    if(RB.row == 0) // read
                        next_state = motor;
                    
                    break;
                case motor:
                    
                    break;
                
                default:
                    
                    break;
            //motor = RB.valstr[0] - '0';
            //ctrl = RB.valstr[1] - '0';
            //memcpy(ctrl,&RB.valstr[1],1*sizeof(char));
            //memcpy(ref,&RB.valstr[2],4*sizeof(char));
            //sprintf(msg,"&motor: %d\tctrl:%d \tref:%d\r\n",motor,ctrl,atoi(ref));
            //UART_PutString(msg);
            }
            break;
        case 'P':
            if(strlen(RB.valstr) > 0){
                updatePID = TRUE;
                PB.pVal = atoi(RB.valstr);
            }   
            break;
        case 'I':
            if(strlen(RB.valstr) > 0){
                updatePID = TRUE;
                PB.iVal = atoi(RB.valstr);
            }
            break;
        case 'D':
            if(strlen(RB.valstr) > 0){
                updatePID = TRUE;
                PB.dVal = atoi(RB.valstr);
            }
            break;
        case 's':
            ContinuouslySendData = TRUE;
            break;
        case 'x':
            ContinuouslySendData = FALSE;
            break;
        case 'q':
            SoftwareReset = TRUE;
            break;/*
        case 'b':
            MOTOR_clearPinBRAKEn(&PM1);
            MOTOR_clearPinBRAKEn(&PM2);
            break;
        case 'B':
            MOTOR_setPinBRAKEn(&PM1);
            MOTOR_setPinBRAKEn(&PM2);
            break;*/
        default:
            break;
    }
    
    switch(motor)
    {
        case 1:
            MOTOR_externControl(&PM1,ctrl,atoi(ref));
            sprintf(msg,"&motor:%d\tctrl:%d\tref:%d\r\n",motor,ctrl,atoi(ref));
            UART_PutString(msg);
            break;
        case 2:
            MOTOR_externControl(&PM2,ctrl,atoi(ref));
            sprintf(msg,"&motor:%d\tctrl:%d\tref:%d\r\n",motor,ctrl,atoi(ref));
            UART_PutString(msg);
            break;
    }
    
    if (RB.cmd == '!'){
        if(strlen(RB.valstr) > 0){
            PM1.ref_rvt = atoi(RB.valstr);
            //PM2.ref_rvt = atoi(RB.valstr);
        }
    }
    else if (RB.cmd == '#'){
        if(strlen(RB.valstr) > 0){
            PM1.ref_spd = atoi(RB.valstr);
            //PM2.ref_spd = atoi(RB.valstr);
        }
    }
    else if (RB.cmd == '$'){
        if(strlen(RB.valstr) > 0){
            PM1.ref_tns = atoi(RB.valstr);
            //PM2.ref_tns = atoi(RB.valstr);
        }
    }
    else if (RB.cmd == '?'){
        if(strlen(RB.valstr) > 0){
            MOTOR_setControlMode(&PM1,atoi(RB.valstr));
            MOTOR_setControlMode(&PM2,atoi(RB.valstr));
            
            sprintf(msg, "!%d%d\r\n",PM1.control_mode,PM1.init_pos);
            UART_PutString(msg);
            if (PM1.control_mode == 1){
                sprintf(msg, "*%d*%d*%d\r\n",(int)(PM1.rvt_params[0]*100.0),(int)(PM1.rvt_params[1]*100.0),(int)(PM1.rvt_params[2]*100.0));     
            }
            if (PM1.control_mode == 2){
                sprintf(msg, "*%d*%d*%d\r\n",(int)(PM1.spd_params[0]*100.0),(int)(PM1.spd_params[1]*100.0),(int)(PM1.spd_params[2]*100.0));    
            }
            if (PM1.control_mode == 3){
                sprintf(msg, "*%d*%d*%d\r\n",(int)(PM1.tns_params[0]*100.0),(int)(PM1.tns_params[1]*100.0),(int)(PM1.tns_params[2]*100.0));
            }
            UART_PutString(msg);
        }
    }
    if ( updatePID == TRUE ){
        if(PM1.control_mode == 1){
            MOTOR_setRvtControlParams(&PM1,(float)PB.pVal/100.0,(float)PB.iVal/100.0,(float)PB.dVal/100.0);
            MOTOR_setRvtControlParams(&PM2,(float)PB.pVal/100.0,(float)PB.iVal/100.0,(float)PB.dVal/100.0);
        }
        else if(PM1.control_mode == 2){
            MOTOR_setSpdControlParams(&PM1,(float)PB.pVal/100.0,(float)PB.iVal/100.0,(float)PB.dVal/100.0);
            MOTOR_setSpdControlParams(&PM2,(float)PB.pVal/100.0,(float)PB.iVal/100.0,(float)PB.dVal/100.0);
        }
        else if(PM1.control_mode == 3){
            MOTOR_setTnsControlParams(&PM1,(float)PB.pVal/100.0,(float)PB.iVal/100.0,(float)PB.dVal/100.0);
            MOTOR_setTnsControlParams(&PM2,(float)PB.pVal/100.0,(float)PB.iVal/100.0,(float)PB.dVal/100.0);
        }
        updatePID = FALSE;
    }
}

/* [] END OF FILE */
