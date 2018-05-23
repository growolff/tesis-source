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

void initGeneralHardware(void){
    
    ADC_TS_Start();    
    UART_Start();
 
    /* Initialize interrupt blocks */
    RxInt_StartEx(MyRxInt);
    
    SPD_COMMAND_ISR_StartEx(SPD_COMMAND_INT);
    RVT_COMMAND_ISR_StartEx(RVT_COMMAND_INT);
    TNS_COMMAND_ISR_StartEx(TNS_COMMAND_INT);
    
    CHECK_MOVEMENT_ISR_StartEx(CHECK_MOVEMENT_INT);

}
void initMotorPM1()
{
    // Initialize hardware associated to motor PM1
    PM1_HA_TIMER_Start();
    PM1_DirCounter_Start();
    PM1_SPD_VDAC8_Start();
    
    // initialize software associated to motor PM1
    PIN_t PM1_BR,PM1_DR,PM1_EN;
    PM1_BR.DR = &PM1_BRAKEn_DR;
    PM1_BR.MASK = PM1_BRAKEn_MASK;
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
void initMotorPM2()
{
    // initialize hardware associated to motor PM2
    PM2_HA_TIMER_Start();
    PM2_DirCounter_Start();
    PM2_SPD_VDAC8_Start();
    
    // initialize software associated to motor PM2
    PIN_t PM2_BR,PM2_DR,PM2_EN;
    PM2_BR.DR = &PM2_BRAKEn_DR;
    PM2_BR.MASK = PM2_BRAKEn_MASK;
    PM2_BR.STATE = 0;
    PM2_DR.DR = &PM2_DIR_DR;
    PM2_DR.MASK = PM2_DIR_MASK;
    PM2_DR.STATE = 1;
    PM2_EN.DR = &PM2_ENABLE_DR;
    PM2_EN.MASK = PM2_ENABLE_MASK;
    PM2_EN.STATE = 1;

    PM2.init_pos = 0;
    PM2_DirCounter_SetCounter(PM2.init_pos);
    
    PM2_HA_ISR_StartEx(PM2_HA_INT);
    MOTOR_init(&PM2,PM2_EN,PM2_BR,PM2_DR);
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
    MOTOR_initControlParams(&PM2,rvt,spd,tns);
    
    initMotorPM1();
    initMotorPM2();
   
}

int main(void)
{
    char TransmitBuffer[TRANSMIT_BUFFER_SIZE];
   
    initGeneralHardware();
    initMotors();

    ADC_TS_StartConvert();
        
    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SoftwareReset = FALSE;

//    PM1_ENABLE_Write(PM1.ENABLE.STATE); // true: speed control, else: external pwm control
//    //SPEED_Write(TRUE); // true: external pwm control, else: analog voltage control
//    PM1_DIR_Write(PM1.DIR.STATE);     // true: left(ccw), false: right(cw)
//    PM1_BRAKEn_Write(PM1.BRAKEn.STATE);  // true: running, false: braken
    
    CyDelay(250);
    
    while(_state_ == 0){
        Ch = UART_GetChar();
        if(Ch == 'n'){
            sprintf(TransmitBuffer, "& INIT CTRL_TYPE = %d\r\n",PM1.control_mode);
            UART_PutString(TransmitBuffer);
            sprintf(TransmitBuffer, "!%d%d\r\n",PM1.control_mode,PM1.init_pos);
            UART_PutString(TransmitBuffer);
            if (PM1.control_mode == 1){
                sprintf(TransmitBuffer, "*%d*%d*%d\r\n",(int)(PM1.rvt_params[0]*100.0),(int)(PM1.rvt_params[1]*100.0),(int)(PM1.rvt_params[2]*100.0));     
            }
            if (PM1.control_mode == 2){
                sprintf(TransmitBuffer, "*%d*%d*%d\r\n",(int)(PM1.spd_params[0]*100.0),(int)(PM1.spd_params[1]*100.0),(int)(PM1.spd_params[2]*100.0));    
            }
            if (PM1.control_mode == 3){
                sprintf(TransmitBuffer, "*%d*%d*%d\r\n",(int)(PM1.tns_params[0]*100.0),(int)(PM1.tns_params[1]*100.0),(int)(PM1.tns_params[2]*100.0));
            }
            UART_PutString(TransmitBuffer);
        }
        if(Ch == 'i'){
            _state_=1;
            break;
        }
    }
    CyGlobalIntEnable; /* Enable global interrupts. */
    //PM1_BRAKEn_Write(0);
    //M_PIN_SET(PM1_BRAKEn);
    //PM1_BRAKEn_DR |= PM1_BRAKEn_MASK; //set BRAKEn pin port
    //PM1_BRAKEn_DR &= ~PM1_BRAKEn_MASK; //clear BRAKEn pin port
    MOTOR_setPinBRAKEn(&PM1);
    MOTOR_setPinDIR(&PM1);
    MOTOR_setPinENABLE(&PM1);
    
    for(;;)
    {
        //sprintf(TransmitBuffer, "& TNS_PID: %d\tDIR: %d\tTNS_CURR: %d\r\n",(int)PM1.tnsPID_out,(int)PM1.DIR.STATE,(int)(PM1.curr_tns));
        //sprintf(TransmitBuffer, "& curr_rvt: %d\tDirState: %d\tinit_pos: %d\r\n",(int)PM1.curr_rvt,(int)PM1.DIR.STATE,(int)(PM1.init_pos));
        //UART_PutString(TransmitBuffer);
        /* Check for PID update */
        while(IsCharReady()){
            //UART_PutString("&IsCharReady\r\n");
            if(GetRxStr()){
                //UART_PutString("&GetRxStr\r\n");
                ProcessCommandMsg();
            }
        }
        ContinuouslySendData = FALSE;
        /* Send data based on last UART command */
        if(ContinuouslySendData)
        {
            /* Format ADC result for transmition */
            if (PM1.control_mode == 0){
                //sprintf(TransmitBuffer, "Ref: %d\tActual: %d\r\n",(int)_pVal,(int)actual_speed);
            }
            else if(PM1.control_mode == 1){
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\tTens: %d\r\n",(int)PM1.ref_rvt,(int)PM1.curr_rvt,(int)(PM1.curr_tns));
            }
            else if(PM1.control_mode == 2){
                //sprintf(TransmitBuffer, "sRef: %d\tsActual: %d\tdirection: %d\tcounter: %d\r\n",(int)speed_ref,(int)actual_speed,(int)dir_state,(int)dir_count);
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\tTens: %d\r\n",(int)PM1.ref_spd,(int)PM1.curr_spd,(int)PM1.curr_tns);
            }
            else if(PM1.control_mode == 3){
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\tTens: %d\r\n",(int)PM1.ref_tns,(int)PM1.curr_tns,(int)PM1.curr_tns);
            }
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
