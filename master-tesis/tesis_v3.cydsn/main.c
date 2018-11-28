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
#include "math.h"

void initGeneralHardware(void)
{    
    SPEED_DAC_Start();
    
    UART_Start();
    ADC_seq_Start();
    ADC_seq_StartConvert();
    /* Initialize interrupt blocks */
    RxInt_StartEx(MyRxInt);
    
}


void initMotorPM1()
{
    // Initialize hardware associated to motor PM1
    PM1_HA_TIMER_Start();
    PM1_DirCounter_Start();
    
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
    
    PM1_HA_ISR_StartEx(PM1_HA_INT);
    RVT_COMMAND_ISR_StartEx(RVT_COMMAND_INT);
    
    MOTOR_init(&PM1,PM1_EN,PM1_DR);
}

void initMotors()
{
    float rvt[3] = {KP_POS, KI_POS, KD_POS};
    float tns[3] = {KP_TENS, KI_TENS, KD_TENS};
    
    MOTOR_initControlParams(&PM1,rvt,tns);    
    initMotorPM1();

    motors[0] = &PM1;
}

int main(void)
{
    _state_ = 0;

    initMotors();
    initGeneralHardware();
    CyGlobalIntEnable; /* Enable global interrupts. */
        
    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SoftwareReset = FALSE;
    
    MOTOR_setPinDIR(&PM1,1);
    MOTOR_setPinENABLE(&PM1,1);

    int pote = 0;
 
    /*while(_state_ == 0){
     CyDelay(500);   
    }*/
    PM1.ref_rvt = 500;
    
    for(;;)
    {
        
        float aux = PM1.rvt_controller.kP;
        //sprintf(TransmitBuffer, "& TNS_PID: %d\tDIR: %d\tTNS_CURR: %d\r\n",(int)PM1.tnsPID_out,(int)PM1.DIR.STATE,(int)(PM1.curr_tns));
        //sprintf(TransmitBuffer, "& curr_rvt: %d\tDirState: %d\tinit_pos: %d\r\n",(int)PM1.curr_rvt,(int)PM1.DIR.STATE,(int)(PM1.init_pos));
        //UART_PutString(TransmitBuffer);
        /* Check for PID update */
        
        /*while(IsCharReady()){
            //UART_PutString("&IsCharReady\r\n");
            if(GetRxStr()){
                //UART_PutString("&GetRxStr\r\n");
                ProcessCommandMsg();
            }
        }
        */
        //pote = ADC_seq_GetResult16(0);
        //SPEED_DAC_SetValue(pote); 
                
        ContinuouslySendData = TRUE;
        /* Send data based on last UART command */
        if(ContinuouslySendData)
        {
            sprintf(TransmitBuffer, "V:%d\tP:%d\tO:%d\tDBG:%.2f\r\n",PM1.curr_spd,PM1.curr_rvt,PM1.ref_spd,aux);
            /* Send out the data */
            UART_PutString(TransmitBuffer);
            
            //sprintf(TransmitBuffer, "P:%d\tI:%d\tD:%d\tPID:%d\n",PM1.rvt_controller.kP,PM1.rvt_controller.kI,PM1.rvt_controller.kD,(int)PM1.rvt_controller.out);
            /* Send out the data */
            //UART_PutString(TransmitBuffer);
            /* Reset the send once flag */
        }
        /*
        if(SoftwareReset){
            CySoftwareReset();
        }*/
        CyDelay(25); // works at 40 Hz
        //PM1.rvt_controller.kP = 1.2;
        
        LED1_Write(_state_);
        _state_ = !_state_;
        
    }
}


/* [] END OF FILE */
