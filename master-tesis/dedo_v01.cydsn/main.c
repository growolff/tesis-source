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
    COUNT_ISR_StartEx(COUNT_INT);
    angle_control_isr_Start();
    tensor_control_isr_Start();

}

void initMotors()
{
    float rvt[3] = {KP_POS, KI_POS, KD_POS}; 
    float spd[3] = {KP_VEL, KI_VEL, KD_VEL}; 
    float tns[3] = {KP_TENS, KI_TENS, KD_TENS};
        
    PM1_HA_ISR_StartEx(PM1_HA_INT);
    MOTOR_setControlParams(&PM1,rvt,spd,tns);
    MOTOR_setControlParams(&PM2,rvt,spd,tns);
    MOTOR_init(&PM1);
    MOTOR_init(&PM2);

}

int main(void)
{
    /*
    // set speed PID parameters 
    float kp_vel = KP_VEL, ki_vel = KI_VEL, kd_vel = KD_VEL;  
    PID_init(&speed_pid_,kp_vel,ki_vel,kd_vel);
    PID_setMaxValue(&speed_pid_, 10000);
    PID_setMinValue(&speed_pid_, 0);
    
    // set position PID parameters 
    float kp_pos = KP_POS, ki_pos = KI_POS, kd_pos = KD_POS;  
    PID_init(&pos_pid_,kp_pos,ki_pos,kd_pos);
    PID_setMaxValue(&pos_pid_, 500);
    PID_setMinValue(&pos_pid_, -500);
    
    // set tension PID parameters
    float kp_tens = KP_TENS, ki_tens = KI_TENS, kd_tens = KD_TENS;  
    PID_init(&tens_pid_,kp_tens,ki_tens,kd_tens);
    PID_setMaxValue(&tens_pid_, 4096);
    PID_setMinValue(&tens_pid_, -4096);
    */
    
    char TransmitBuffer[TRANSMIT_BUFFER_SIZE];
   
    initHardware();
    initMotors();

    ADC_StartConvert();
    ADC_TS_StartConvert();
        
    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SendSingleByte = FALSE;
    SendEmulatedData = FALSE;

    PM1_ENABLE_Write(PM1.ENABLE); // true: speed control, else: external pwm control
    //SPEED_Write(TRUE); // true: external pwm control, else: analog voltage control
    PM1_DIR_Write(PM1.DIR);     // true: left(ccw), false: right(cw)
    PM1_BRAKEn_Write(FALSE);  // true: turning, false: braken
    
    CyDelay(250);
    
    dir_state = TRUE; //left, negative counter
    
    /* Set initial position of rotor */
    init_pos = 0; 
    /* Set reference position for control */
    pos_ref = 0;
    /* Set reference tension for control */
    tension_ref = 2048;    
    
    debug = 0;
    
    while(_state_ == 0){
        Ch = UART_GetChar();
        if(Ch == 'i'){
            sprintf(TransmitBuffer, "& INIT CTRL_TYPE = %d\r\n",CONTROL_TYPE);
            UART_PutString(TransmitBuffer);
            sprintf(TransmitBuffer, "!%d\r\n",CONTROL_TYPE);
            UART_PutString(TransmitBuffer);
            #if CONTROL_TYPE==1
                sprintf(TransmitBuffer, "*%d*%d*%d\r\n",(int)(kp_pos*100.0),(int)(ki_pos*100.0),(int)(kd_pos*100.0));     
            #elif CONTROL_TYPE==2
                sprintf(TransmitBuffer, "*%d*%d*%d\r\n",(int)(PM1.spd_params[0]*100.0),(int)(PM1.spd_params[1]*100.0),(int)(PM1.spd_params[2]*100.0));    
            #elif CONTROL_TYPE==3
                sprintf(TransmitBuffer, "*%d*%d*%d\r\n",(int)(kp_tens*100.0),(int)(ki_tens*100.0),(int)(kd_tens*100.0));
            #endif
            UART_PutString(TransmitBuffer);
            _state_=1;
            break;
        }
    }
    CyGlobalIntEnable; /* Enable global interrupts. */
    PM1_BRAKEn_Write(PM1.BRAKEn);
    
    for(;;)
    {
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
        
        #ifdef MANUAL_CONTROL
            VDAC8_Speed_SetValue((uint8)(_pVal));
        #endif
        #ifdef SPEED_CONTROL
            //speed_ref = fn_mapper(_pVal,0,255,0,10000);        
        #endif
                
        /* change motor direction */
        PM1_DIR_Write(PM1.DIR);
        
        #ifdef TENDON_TENSION_CONTROL
            /* rotor zero crossing checks */
            if(actual_pos >= 0 && dir_state == 0){ 
                BRAKEn_Write(0);
            }
            else if(dir_state == 1){
                BRAKEn_Write(1);
            }
        #endif
        
        if( abs(PM1.ref_spd - PM1.curr_spd) > 0 ){
            MOTOR_setSpdRef(&PM1,PM1.ref_spd);
        }
        
        /* Check UART for any sent command */
        Ch = UART_GetChar();
        
        /* Send data based on last UART command */
        if(SendSingleByte || ContinuouslySendData)
        {
            /* Format ADC result for transmition */
            #ifdef MANUAL_CONTROL
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\r\n",(int)_pVal,(int)actual_speed);
            #elif defined(ANGLE_CONTROL)
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\tpid_out: %d\r\n",(int)pos_ref*4,(int)actual_pos,(int)_pid_out);
            #elif defined(SPEED_CONTROL)
                //sprintf(TransmitBuffer, "sRef: %d\tsActual: %d\tdirection: %d\tcounter: %d\r\n",(int)speed_ref,(int)actual_speed,(int)dir_state,(int)dir_count);
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\r\n",(int)PM1.ref_spd,(int)PM1.curr_spd);
            #elif defined(TENDON_TENSION_CONTROL)
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\r\n",(int)tension_ref,(int)actual_tension);
            #endif
            /* Send out the data */
            UART_PutString(TransmitBuffer);
            /* Reset the send once flag */
            SendSingleByte = FALSE;
        }
        else if(SendEmulatedData)
        {
            /* Format ADC result for transmition */
            sprintf(TransmitBuffer, "Dir: %s \r\n", (dir_state?"Left":"Right"));
            /* Send out the data */
            UART_PutString(TransmitBuffer);
            /* Reset the send once flag */
            SendEmulatedData = FALSE;   
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
