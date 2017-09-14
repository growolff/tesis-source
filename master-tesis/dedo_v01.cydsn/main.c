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
#include "functions.h"

void initHardware(void){
    
    ADC_Start();
    ADC_tension_Start();
    Timer_ha_Start();
    DirCounter_Start();
    VDAC8_Speed_Start();
    UART_Start();
    
    /* Initialize interrupt blocks */
    RxInt_StartEx(MyRxInt);
    isr_HA_Start();
    isr_counter_Start();
    isr_button_Start();
    vel_control_isr_Start();
    angle_control_isr_Start();
    tensor_control_isr_Start();

}

int main(void)
{
    /* set speed PID parameters */
    float kp_vel = KP_VEL, ki_vel = KI_VEL, kd_vel = KD_VEL;  
    PID_init(&speed_pid_,kp_vel,ki_vel,kd_vel);
    PID_setMaxValue(&speed_pid_, 10000);
    PID_setMinValue(&speed_pid_, 0);
    
    /* set position PID parameters */
    float kp_pos = KP_POS, ki_pos = KI_POS, kd_pos = KD_POS;  
    PID_init(&pos_pid_,kp_pos,ki_pos,kd_pos);
    PID_setMaxValue(&pos_pid_, 500);
    PID_setMinValue(&pos_pid_, -500);
    
    /* set tension PID parameters */
    float kp_tens = KP_TENS, ki_tens = KI_TENS, kd_tens = KD_TENS;  
    PID_init(&tens_pid_,kp_tens,ki_tens,kd_tens);
    PID_setMaxValue(&tens_pid_, 4096);
    PID_setMinValue(&tens_pid_, -4096);
    
    int16 dir_count;
    
    char TransmitBuffer[TRANSMIT_BUFFER_SIZE];
   
    initHardware();

    ADC_StartConvert();
    ADC_tension_StartConvert();
        
    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SendSingleByte = FALSE;
    SendEmulatedData = FALSE;
        
    ENABLE_Write(TRUE); // true: speed control, else: external pwm control
    //SPEED_Write(TRUE); // true: external pwm control, else: analog voltage control
    DIR_Write(1);     // true: left(ccw), false: right(cw)
    BRAKEn_Write(FALSE);  // true: turning, false: braken
    
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
            #if CONTROL_TYPE==1
                sprintf(TransmitBuffer, "*%d*%d*%d\r\n",(int)(kp_pos*100.0),(int)(ki_pos*100.0),(int)(kd_pos*100.0));     
            #elif CONTROL_TYPE==2
                sprintf(TransmitBuffer, "*%d*%d*%d\r\n",(int)(kp_vel*100.0),(int)(ki_vel*100.0),(int)(kd_vel*100.0));    
            #elif CONTROL_TYPE==3
                sprintf(TransmitBuffer, "*%d*%d*%d\r\n",(int)(kp_tens*100.0),(int)(ki_tens*100.0),(int)(kd_tens*100.0));
            #endif
            UART_PutString(TransmitBuffer);
            _state_=1;
            break;
        }
    }
    CyGlobalIntEnable; /* Enable global interrupts. */
    BRAKEn_Write(1);
    
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
        
        dir_count = DirCounter_GetCounter();
        /* Place your application code here. */
        _pVal = ADC_GetResult8();
        _tVal = ADC_tension_GetResult16();
        
        #ifdef MANUAL_CONTROL
            VDAC8_Speed_SetValue((uint8)(_pVal));
        #endif
        #ifdef SPEED_CONTROL
            //speed_ref = fn_mapper(_pVal,0,255,0,10000);        
        #endif
                
        /* change motor direction */
        DIR_Write(dir_state);
        
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
        
        /* Set flags based on UART command */
        switch(Ch)
        {
            case 0:
                /* No new data was recieved */
                break;
            case 'C':
            case 'c':
                SendSingleByte = TRUE;
                break;
            case 'S':
            case 's':
                ContinuouslySendData = TRUE;
                break;
            case 'X':
            case 'x':
                ContinuouslySendData = FALSE;
                break;
            case 'E':
            case 'e':
                SendEmulatedData = TRUE;
                break;
            case 'q':
                _uart_Reset = TRUE;
                break;
            case 'b':
                BRAKEn_Write(FALSE);
                break;
            case '1':
                pos_ref = -30;
                break;
            case '2':
                pos_ref = -15;
                break;
            case '3':
                pos_ref = 15;
                break;
            case '4':
                pos_ref = 30;
                break;
            case '0':
                pos_ref = 0;
                break;
            default:
                /* Place error handling code here */
                break;    
        }
        
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
                sprintf(TransmitBuffer, "Ref: %d\tActual: %d\r\n",(int)speed_ref,(int)actual_speed);
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
