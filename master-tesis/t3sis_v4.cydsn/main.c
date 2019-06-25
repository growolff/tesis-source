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
    sprintf(strMsg,"*%s\r\n", data);
    UART_PutString(strMsg);   
}
void echod(int data)
{
    sprintf(strMsg,"*%d\r\n", data);
    UART_PutString(strMsg);   
}

void echof(float data)
{
    sprintf(strMsg,"*%.4f\r\n", data);
    UART_PutString(strMsg);  
}

void initMotorPM1()
{
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
    
    
    PM1.spd_pid[0] = 0.9;
    PM1.spd_pid[1] = 0.016;
    PM1.spd_pid[2] = 0.0;
    
    PM1.rvt_pid[0] = 1.0;
    PM1.rvt_pid[1] = 0.0;
    PM1.rvt_pid[2] = 0.0;
    
    /*
    PM1.spd_pid[0] = (float)CY_GET_REG32(CYDEV_EE_BASE);
    PM1.spd_pid[1] = (float)CY_GET_REG32(CYDEV_EE_BASE+0x10);
    PM1.spd_pid[2] = (float)CY_GET_REG32(CYDEV_EE_BASE+0x20);
    
    PM1.rvt_params[0] = CY_GET_REG32(CYDEV_EE_BASE+0x30);
    PM1.rvt_params[1] = CY_GET_REG32(CYDEV_EE_BASE+0x40);
    PM1.rvt_params[2] = CY_GET_REG32(CYDEV_EE_BASE+0x50);
*/
    MOTOR_init(&PM1,PM1_EN,PM1_DR);
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
    HA_ISR_StartEx(PM1_HA_INT);
    RVT_COMMAND_ISR_StartEx(RVT_COMMAND_INT); //revolutions control isr
    SPD_COMMAND_ISR_StartEx(SPD_COMMAND_INT); //speed control isr
    
    _state_ = 0;
    initMotor();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    MOTOR_setPinDIR(&PM1,0);
    MOTOR_setPinENABLE(&PM1,0);
    
    motors[0]->control_mode = 0;
    
    ContinuouslySendData = FALSE;
    
    /* transmition rate */
    int rate_ms = 1000/RATE_HZ;
    uint8_t pote = 0;
    
    uint16_t actual_time = millis_ReadCounter();
    echof(motors[0]->spd_pid[0]);
    
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
                if(motors[0]->control_mode == 0){
                    WB.ref = motors[0]->ref_spd;
                    WB.cur = motors[0]->curr_spd;
                    WB.val = 0;
                }
                else if(motors[0]->control_mode == 1){
                    WB.ref = motors[0]->ref_rvt;
                    WB.cur = motors[0]->curr_rvt;
                    WB.val = 0;
                }
               
                UART_PutArray((const uint8*)&WB.buffStr,len);
            }
        }
    }
}

/* [] END OF FILE */
