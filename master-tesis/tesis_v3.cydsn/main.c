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
    PIN_t PM1_BR,PM1_DR,PM1_EN;
    PM1_BR.STATE = 0;
    PM1_DR.DR = &PM1_DIR_DR;
    PM1_DR.MASK = PM1_DIR_MASK;
    PM1_DR.STATE = 0;
    PM1_EN.DR = &PM1_ENABLE_DR;
    PM1_EN.MASK = PM1_ENABLE_MASK;
    PM1_EN.STATE = 0;
    
    PM1.init_pos = 0;
    PM1_DirCounter_SetCounter(PM1.init_pos);
    
    PM1_HA_ISR_StartEx(PM1_HA_INT);
    
    MOTOR_init(&PM1,PM1_EN,PM1_BR,PM1_DR);
}

void initMotors()
{
    float rvt[3] = {KP_POS, KI_POS, KD_POS}; 
    float spd[3] = {KP_VEL, KI_VEL, KD_VEL}; 
    float tns[3] = {KP_TENS, KI_TENS, KD_TENS};
   
    PM1.L = 40.0;
    PM1.A = 1.5;
    PM1.R = 0.19;
    
    MOTOR_initControlParams(&PM1,rvt,spd,tns);    
    initMotorPM1();

    
    motors[0] = &PM1;
}

int main(void)
{
    
    initMotors();
    _state_ = 0;
   
    initGeneralHardware();
    CyGlobalIntEnable; /* Enable global interrupts. */
        
    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SoftwareReset = FALSE;
    
    MOTOR_setPinDIR(&PM1,1);

    int pote = 0;
    
    for(;;)
    {

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
        }*/
        
        pote = ADC_seq_GetResult16(0);
        SPEED_DAC_SetValue(pote);
        
        ContinuouslySendData = TRUE;
        /* Send data based on last UART command */
        if(ContinuouslySendData)
        {
            sprintf(TransmitBuffer, "vel: %d\r\n",(int)PM1.curr_spd);
            /* Send out the data */
            UART_PutString(TransmitBuffer);
            /* Reset the send once flag */
        }
        if(SoftwareReset){
            CySoftwareReset();
        }
        CyDelay(25); // works at 40 Hz
        
        //MOTOR_checkError(&PM1);
    }
}


/* [] END OF FILE */
