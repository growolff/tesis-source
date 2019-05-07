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
    
    PM1.spd_params[0] = 1.0;
    PM1.spd_params[1] = 1.0;
    //PM1.spd_params[1] = (float)EEPROM_1_ReadByte(4)*0.01;
    PM1.spd_params[2] = (float)EEPROM_1_ReadByte(5)*0.01;
    
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
    
    SPD_COUNTER_Start();
    SPEED_PWM_Start();
    POTE_ADC_Start();
    POTE_ADC_StartConvert();
    //SPEED_DAC_Start();
    UART_Start();
    EEPROM_1_Start();
    /* Initialize interrupt blocks */
    RxInt_StartEx(MyRxInt);
    
    // Initialize hardware related to motor PM1
    PM1_HA_TIMER_Start();
    PM1_DirCounter_Start();
    // Initialize ISR
    //PM1_DirCounter_isr_StartEx(PM1_HA_INT);
    HA_ISR_StartEx(PM1_HA_INT);
    RVT_COMMAND_ISR_StartEx(RVT_COMMAND_INT); //revolutions control isr
    SPD_COMMAND_ISR_StartEx(SPD_COMMAND_INT); //speed control isr
    
    _state_ = 0;
    factor = 100.0;

    initMotor();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    MOTOR_setPinDIR(&PM1,0);
    MOTOR_setPinENABLE(&PM1,1);
        
    PM1.ref_rvt = 0;
    motors[0]->control_mode = 0;
    
    ContinuouslySendData = TRUE;
    
    /* transmition rate */
    int rate_ms = 1000/RATE_HZ;
    uint8_t pote = 0;
    
    uint16_t actual_time = millis_ReadCounter();
    
    //SPEED_DAC_SetValue(0);
    motors[0]->control_mode = 0;

    for(;;)
    {
        /* Check motor direction and rotation */
//        MOTOR_checkDir(motors[0]);
        
        while(IsCharReady()){
            //UART_PutString("&IsCharReady\r\n");
            if(GetRxStr()){
                //UART_PutString("&GetRxStr\r\n");
                ProcessCommandMsg();
            }
        }
        
        pote = POTE_ADC_GetResult8();
        //motors[0]->ref_spd = fn_mapper(pote,0,255,0,9000);
        SPEED_PWM_WriteCompare(pote);

        if(millis_ReadCounter() - actual_time > rate_ms)
        {   
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
                sprintf(TransmitBuffer, "V:%d\tP:%d\tO:%d\r\n",pote,motors[0]->spdPID_out,motors[0]->curr_spd);
                UART_PutString(TransmitBuffer);
                //sprintf(TransmitBuffer, "Ref: %d\tActual: %d\tTens: %d\r\n",(int)PM1.ref_rvt,(int)PM1.curr_rvt,(int)motors[0]->rvt_controller.kP*1000);
                
                //sprintf(TransmitBuffer, "%d\t%d\t%d\r\n",PM1.ref_rvt,PM1.curr_rvt,PM1.ref_spd);
                /* Send out the data */

            }
        }
    }
}

/* [] END OF FILE */