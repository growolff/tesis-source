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

//#define M_PIN_SET(pin) #pin"_DR |=" #pin"_MASK"

void initHardware(void){
    
    ADC_Start();
    ADC_TS_Start();
    PM1_HA_TIMER_Start();
    PM1_DirCounter_Start();
    PM1_SPD_VDAC8_Start();
    UART_Start();
    
    /* Initialize interrupt blocks */
    RxInt_StartEx(MyRxInt);
    
    SPD_COMMAND_ISR_StartEx(SPD_COMMAND_INT);
    RVT_COMMAND_ISR_StartEx(RVT_COMMAND_INT);
    COUNT_ISR_StartEx(COUNT_INT);

    tensor_control_isr_Start();

}

void initMotors()
{
    float rvt[3] = {KP_POS, KI_POS, KD_POS}; 
    float spd[3] = {KP_VEL, KI_VEL, KD_VEL}; 
    float tns[3] = {KP_TENS, KI_TENS, KD_TENS};
        
    PIN_t PM1_BR,PM1_DR,PM1_EN;

    PM1_BR.DR = &PM1_BRAKEn_DR;
    PM1_BR.MASK = PM1_BRAKEn_MASK;
    PM1_BR.STATE = 0;
    PM1_DR.DR = &PM1_DIR_DR;
    PM1_DR.MASK = PM1_DIR_MASK;
    PM1_DR.STATE = 1;
    PM1_EN.DR = &PM1_ENABLE_DR;
    PM1_EN.MASK = PM1_ENABLE_MASK;
    PM1_EN.STATE = 1;
    
    PM1.control_mode = 2;
    
    PM1_HA_ISR_StartEx(PM1_HA_INT);
    //MOTOR_setControlParams(&PM2,rvt,spd,tns);
    MOTOR_initControlParams(&PM1,rvt,spd,tns);
    MOTOR_init(&PM1,PM1_EN,PM1_BR,PM1_DR);
    
    //MOTOR_init(&PM2);

}

int main(void)
{
    char TransmitBuffer[TRANSMIT_BUFFER_SIZE];
   
    initHardware();
    initMotors();

    ADC_StartConvert();
    ADC_TS_StartConvert();
        
    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SendSingleByte = FALSE;
    SendEmulatedData = FALSE;

    PM1_ENABLE_Write(PM1.ENABLE.STATE); // true: speed control, else: external pwm control
    //SPEED_Write(TRUE); // true: external pwm control, else: analog voltage control
    PM1_DIR_Write(PM1.DIR.STATE);     // true: left(ccw), false: right(cw)
    PM1_BRAKEn_Write(PM1.BRAKEn.STATE);  // true: turning, false: braken
    
    CyDelay(250);
    
    /* Set initial position of rotor */
    //init_pos = 0; 
    /* Set reference position for control */
    //pos_ref = 0;
    /* Set reference tension for control */
    //tension_ref = 2048;    
    
    debug = 0;
    
    while(_state_ == 0){
        Ch = UART_GetChar();
        if(Ch == 'n'){
            sprintf(TransmitBuffer, "& INIT CTRL_TYPE = %d\r\n",PM1.control_mode);
            UART_PutString(TransmitBuffer);
            sprintf(TransmitBuffer, "!%d\r\n",PM1.control_mode);
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
    
    for(;;)
    {
        sprintf(TransmitBuffer, "& REF: %d\tCUR: %d\tPID: %d\r\n",PM1.ref_rvt,PM1.curr_rvt,PM1.rvtPID_out);
        UART_PutString(TransmitBuffer);
        /* Check for PID update */
        while(IsCharReady()){
            //UART_PutString("&IsCharReady\r\n");
            if(GetRxStr()){
                //UART_PutString("&GetRxStr\r\n");
                ProcessCommandMsg();
            }
        }
        if(ADC_TS_IsEndConversion(ADC_TS_RETURN_STATUS)!=0){
            TS_array = StoreResults();
        }
        
        //_pVal = ADC_GetResult8();
        _tVal = TS_array[0];
        
        #ifdef TENDON_TENSION_CONTROL
            /* rotor zero crossing checks */
            if(actual_pos >= 0 && dir_state == 0){ 
                BRAKEn_Write(0);
            }
            else if(dir_state == 1){
                BRAKEn_Write(1);
            }
        #endif
        
        /* Check UART for any sent command */
        Ch = UART_GetChar();
        
        /* Send data based on last UART command */
        if(SendSingleByte || ContinuouslySendData)
        {
            /* Format ADC result for transmition */
            if (PM1.control_mode == 0){
                //sprintf(TransmitBuffer, "Ref: %d\tActual: %d\r\n",(int)_pVal,(int)actual_speed);
            }
            else if(PM1.control_mode == 1){
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\r\n",(int)PM1.ref_rvt,(int)PM1.curr_rvt);
            }
            else if(PM1.control_mode == 2){
                //sprintf(TransmitBuffer, "sRef: %d\tsActual: %d\tdirection: %d\tcounter: %d\r\n",(int)speed_ref,(int)actual_speed,(int)dir_state,(int)dir_count);
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\r\n",(int)PM1.ref_spd,(int)PM1.curr_spd);
            }
            else if(PM1.control_mode == 3){
                //sprintf(TransmitBuffer, "Ref: %d\tActual: %d\r\n",(int)tension_ref,(int)actual_tension);
            }
            /* Send out the data */
            UART_PutString(TransmitBuffer);
            /* Reset the send once flag */
            SendSingleByte = FALSE;
        }

        if(_uart_Reset)
        {
            /* Reset the psoc using software */
            CySoftwareReset();
        }
                
        CyDelay(25); // works at 40 Hz
    }
}


/* [] END OF FILE */
