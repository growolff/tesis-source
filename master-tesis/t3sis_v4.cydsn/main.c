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
    PM1.control_mode = 1;
    PM1_DirCounter_SetCounter(PM1.init_pos);
    
    MOTOR_init(&PM1,PM1_EN,PM1_DR);
}

void initMotor()
{       
    initMotorPM1();

    motors[0] = &PM1;
    
    MOTOR_setRvtControlParams(motors[0],KP_POS,KI_POS,KD_POS);
}

int main(void)
{
    SPEED_DAC_Start();
    UART_Start();
    /* Initialize interrupt blocks */
    RxInt_StartEx(MyRxInt);
    
    // Initialize hardware related to motor PM1
    PM1_HA_TIMER_Start();
    PM1_DirCounter_Start();
    // Initialize ISR
    PM1_HA_ISR_StartEx(PM1_HA_INT);
    RVT_COMMAND_ISR_StartEx(RVT_COMMAND_INT);
    
    _state_ = 0;

    initMotor();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    MOTOR_setPinDIR(&PM1,0);
    MOTOR_setPinENABLE(&PM1,1);
    
    PM1.ref_rvt = 200;
    
    ContinuouslySendData = FALSE;
    
    /* transmition rate */
    int rate = 2;
    int count = 0;

    for(;;)
    {
        while(IsCharReady()){
            //UART_PutString("&IsCharReady\r\n");
            if(GetRxStr()){
                //UART_PutString("&GetRxStr\r\n");
                ProcessCommandMsg();
            }
        }
        
        /* Send data based on last UART command */
        if(ContinuouslySendData)
        {
            //sprintf(TransmitBuffer, "V:%d\tP:%d\tO:%d\r\n",PM1.curr_spd,PM1.curr_rvt,PM1.ref_spd);
            sprintf(TransmitBuffer, "Ref: %d\tActual: %d\tTens: %d\r\n",(int)PM1.ref_rvt,(int)PM1.curr_rvt,(int)(PM1.rvt_controller.kP));
            
            //sprintf(TransmitBuffer, "%d\t%d\t%d\r\n",PM1.ref_rvt,PM1.curr_rvt,PM1.ref_spd);
            /* Send out the data */
            
            if (count%rate == 0)
            {
                UART_PutString(TransmitBuffer);
                count = 0;
            }
       
        }
        CyDelay(25); // works at 40 Hz
        
        LED1_Write(_state_);
        _state_ = !_state_;
        
        count++;
    }
}

/* [] END OF FILE */
