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
#include "global.h"

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

void writeEEPROM()
{
    if(EEPROM_1_WriteByte(100,3)){
        //echo("writebyte1");
    }
    if(EEPROM_1_WriteByte(20,4)){
        //echo("writebyte2");
    }
    if(EEPROM_1_WriteByte(0,5)){
        //echo("writebyte2");
    }
    
}

void initMotorPM1()
{
    
    writeEEPROM();
    // initialize software associated to motor PM1
    PIN_t PM1_DR,PM1_EN;
    PM1_DR.DR = &PM1_DIR_DR;
    PM1_DR.MASK = PM1_DIR_MASK;
    PM1_DR.STATE = 0;
    PM1_EN.DR = &PM1_ENABLE_DR;
    PM1_EN.MASK = PM1_ENABLE_MASK;
    PM1_EN.STATE = 0;
    
    PM1.init_pos = 0;
    PM1.control_mode = 0;
    PM1_DirCounter_SetCounter(PM1.init_pos);
    
    pid_rvt[0] = EEPROM_1_ReadByte(0);
    pid_rvt[1] = EEPROM_1_ReadByte(1);
    pid_rvt[2] = EEPROM_1_ReadByte(2);
    
    PM1.spd_params[0] = 2;
    PM1.spd_params[1] = 0.01;
    PM1.spd_params[2] = 0.0;
    //PM1.spd_params[1] = (float)EEPROM_1_ReadByte(4)*0.01;
    //PM1.spd_params[2] = (float)EEPROM_1_ReadByte(5)*0.01;
    
    MOTOR_init(&PM1,PM1_EN,PM1_DR);
     
    //MOTOR_setRvtControlParams(motors[0],(float)pid_rvt[0]*0.01,(float)pid_rvt[1]*0.01,(float)pid_rvt[2]*0.01);
    //MOTOR_setSpdControlParams(motors[0],(float)pid_spd[0]*0.01,(float)pid_spd[1]*0.01,(float)pid_spd[2]*0.01);
    //MOTOR_setSpdControlParams(motors[0],(float)1.0,0,0);
}

void initMotor()
{       
    initMotorPM1();
    
    motors[0] = &PM1;
}

int main(void)
{
    millis_Start();
    
    SPEED_PWM_Start();
    POTE_ADC_Start();
    POTE_ADC_StartConvert();
    UART_Start();
    EEPROM_1_Start();
    /* Initialize interrupt blocks */
    RxInt_StartEx(MyRxInt);
    
    // Initialize hardware related to motor PM1
    PM1_HA_TIMER_Start();
    PM1_DirCounter_Start();
    // Initialize ISR
    HA_ISR_StartEx(PM1_HA_INT);
    RVT_COMMAND_ISR_StartEx(RVT_COMMAND_INT); //revolutions control isr
    SPD_COMMAND_ISR_StartEx(SPD_COMMAND_INT); //speed control isr
    
    _state_ = 0;
    factor = 100.0;

    initMotor();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    MOTOR_setPinDIR(&PM1,0);
    MOTOR_setPinENABLE(&PM1,1);
    
    motors[0]->control_mode = 0;
    
    ContinuouslySendData = TRUE;
    
    /* transmition rate */
    int rate_ms = 1000/RATE_HZ;
    uint8_t pote = 0;
    
    uint16_t actual_time = millis_ReadCounter();
    
    //union comMsg COM;
      
    uint8_t c;
    
    for(;;)
    {        
        while(IsCharReady()){
            //UART_PutString("&IsCharReady\r\n");
            if(GetRxStr()){
                //UART_PutString("&GetRxStr\r\n");
                ProcessCommandMsg();
            }
        }
        
        pote = POTE_ADC_GetResult8();
        //SPEED_PWM_WriteCompare(pote);

        if(millis_ReadCounter() - actual_time > rate_ms)
        {   
            // set speed reference (only for testing)
            //motors[0]->ref_spd = fn_mapper(pote,0,255,0,9000);
            
            //CyDelay(25); // works at 40 Hz
            _state_ = !_state_;
            LED1_Write(_state_);
            
            actual_time = millis_ReadCounter();
            if(actual_time > 60000)
            {
                millis_WriteCounter(0); //resetea el contador cuando pasa los 60 segundos
                actual_time = millis_ReadCounter();
            }

            /* Send data based on last UART command */
            if(ContinuouslySendData)
            {
                int len = sizeof(WB.buffStr)/sizeof(*WB.buffStr);
                WB.xff = 0xff;
                WB.cmd = 1;
                //WB.ref = c;
                //WB.cur = c;
                WB.ref = motors[0]->ref_spd;
                WB.cur = motors[0]->curr_spd;
                WB.val = 0;
               
                //sprintf(TransmitBuffer, "P:%d\tO:%d\r\n",motors[0]->ref_spd,motors[0]->curr_spd);
                //int ref = motors[0]->control_mode == 0 ? motors[0]->ref_spd : motors[0]->ref_rvt ;
                //int curr = motors[0]->control_mode == 0 ? motors[0]->curr_spd: motors[0]->curr_rvt ;
                //sprintf(TransmitBuffer, "%d%d%d",(int)motors[0]->ref_spd,(int)motors[0]->curr_spd,actual_time);
                //sprintf(TransmitBuffer,"%01x%02x%02x",COM.cmd,COM.ref,COM.cur);
                //sprintf(TransmitBuffer,"%s",COM.buffStr);
                /* Send out the data */
                //UART_PutString(TransmitBuffer);
                
                UART_PutArray((const uint8*)&WB.buffStr,len);
                /*
                for(uint i=0; i < strlen(COM.buffStr); i++){
                    strcpy(TransmitBuffer,"\"");
                    UART_WriteTxData([i]);
                }
                */
            }
        }
    }
}

/* [] END OF FILE */
