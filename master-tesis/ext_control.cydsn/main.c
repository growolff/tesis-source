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

int main(void)
{
    /* Variable to store UART received character */
    uint8 Ch='S';
    /* Flags used to store transmit data commands */
    uint8 ContinuouslySendData;
    uint8 SendSingleByte;
    uint8 SendEmulatedData;
    uint8 Reset=FALSE;
    uint8 Turn,Turn_serial,TurnLeft,TurnRight;
    int _state_=0;
    
    int16 dir_count;
    
    char TransmitBuffer[TRANSMIT_BUFFER_SIZE];
    
    /* set speed PID parameters */
    float kp_vel = KP_VEL, ki_vel = KI_VEL, kd_vel = KD_VEL;  
    PID_init(&speed_pid_,kp_vel,ki_vel,kd_vel);
    PID_setMaxValue(&speed_pid_, 12000);
    PID_setMinValue(&speed_pid_, 0);
    
    /* set position PID parameters */
    float kp_pos = KP_POS, ki_pos = KI_POS, kd_pos = KD_POS;  
    PID_init(&pos_pid_,kp_pos,ki_pos,kd_pos);
    PID_setMaxValue(&pos_pid_, 10000);
    PID_setMinValue(&pos_pid_, 0);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    ADC_Start();
    ADC_tension_Start();
    PWM_Start();        // initialize PWM block
    UART_1_Start();
    Timer_ha_Start();
    DirCounter_Start();

    ADC_StartConvert();
    ADC_tension_StartConvert();
    PWM_Enable();       // enable PWM generation
    
    isr_HA_Start();
    isr_counter_Start();
    isr_button_Start();
    vel_control_isr_Start();
    angle_control_isr_Start();

    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SendSingleByte = FALSE;
    SendEmulatedData = FALSE;
    
    Turn_serial = FALSE;
    Turn = TRUE;
    TurnLeft = FALSE;
    TurnRight = FALSE;
    
    //ENABLE_Write(1); // true: speed control, else: external pwm control
    SPEED_Write(TRUE); // true: external pwm control, else: analog voltage control
    //DIR_Write(1);
    BRAKEn_Write(TRUE);
    
    current_pos = 0; /* Set initial position of rotor */
    pos_ref = 100;
    
    UART_1_PutString("----- COM5 Port Open -----\r\n");
    
    dir_state = 1; //left
    
    while(_state_ == 0){
        Ch = UART_1_GetChar();
        if(Ch == 'i'){
            _state_=1;
            UART_1_PutString("[INFO] Start \r\n");
            break;
        }
    }

    for(;;)
    {
        dir_count = DirCounter_GetCounter();
        /* Place your application code here. */
        _pVal = ADC_GetResult8();
        _tVal = ADC_tension_GetResult16();
        
        #ifdef MANUAL_CONTROL
            PWM_WriteCompare((uint8)(255-_pVal));
            //speed_ref = fn_mapper(_pVal,0,255,0,9500);        
        #endif
                
        /* change motor direction with button */
        DIR_Write(dir_state);
        
        /* Check UART for any sent command */
        Ch = UART_1_GetChar();
        
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
            case 'l':
                TurnLeft = TRUE;
                break;
            case 'r':
                TurnRight = TRUE;
                break;
            case ' ':
                Turn_serial = TRUE;
                break;
            case 'q':
                Reset = TRUE;
                break;
            case '1':
                pos_ref = 15;
                break;
            case '2':
                pos_ref = 50;
                break;
            case '3':
                pos_ref = -50;
                break;
            default:
                /* Place error handling code here */
                break;    
        }
        
        /* Send data based on last UART command */
        if(SendSingleByte || ContinuouslySendData)
        {
            /* Format ADC result for transmition */
            //sprintf(TransmitBuffer, "counter: %d\t mean: %d  \t debug: %d \r\n",(int)_vel_counter,(int)_last_median, (int)debug);
            sprintf(TransmitBuffer, "count: %d \t pid: %d\t vel: %d [rpm]\r\n",(int)dir_count,(int)rotor_direction,(int)current_speed);
            //sprintf(TransmitBuffer, "%d,%d,%d\n", _motor_speed*50/16, (int)(_speed_ref)*50/16, _pid_output);
            /* Send out the data */
            UART_1_PutString(TransmitBuffer);
            /* Reset the send once flag */
            SendSingleByte = FALSE;
        }
        else if(SendEmulatedData)
        {
            /* Format ADC result for transmition */
            sprintf(TransmitBuffer, "Dir: %s \tTurning: %s \r\n", dir_state?"Left":"Right", Turn?"Turn":"Stop");
            /* Send out the data */
            UART_1_PutString(TransmitBuffer);
            /* Reset the send once flag */
            SendEmulatedData = FALSE;   
        }
        
        if(Reset)
        {
            /* Reset the psoc using software */
            CySoftwareReset();
        }
        if(Turn_serial)
        {
            uint8 brakeState = BRAKEn_Read();
            BRAKEn_Write(~ brakeState);
            Turn = ~ Turn;
            Turn_serial = FALSE;
        }
        if(TurnLeft)
        {
            dir_state = 1; // Left, Counter ClockWise
            sprintf(TransmitBuffer, "Turn: CCW (L)\r\n");
            UART_1_PutString(TransmitBuffer);
            TurnLeft = FALSE;
        }
        else if(TurnRight)
        {
            dir_state = 0; // Right, ClockWise
            sprintf(TransmitBuffer, "Turn: CW (R)\r\n");
            UART_1_PutString(TransmitBuffer);
            TurnRight = FALSE;
        }
                
        CyDelay(20); // works at 50 Hz
    }
}

/* [] END OF FILE */
