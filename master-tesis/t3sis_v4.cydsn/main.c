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

void initMotor1()
{
    // Initialize hardware related to motor M1
    PWM_M1_Start();
    DC_M1_Start();

    // initialize software associated to motor M1
    PIN_t M1_DR,M1_EN;
    M1_DR.DR = &M1_DIR_DR;
    M1_DR.MASK = M1_DIR_MASK;
    M1_DR.STATE = 0;
    M1_EN.DR = &M1_EN_DR;
    M1_EN.MASK = M1_EN_MASK;
    M1_EN.STATE = 0;

    M1.init_pos = 0;
    M1.control_mode = 1;
    M1.idx = I_ME_IDX;         // motor index
    DC_M1_SetCounter(M1.init_pos);

    M1.rvt_pid[0] = M1_KP;
    M1.rvt_pid[1] = M1_KI;
    M1.rvt_pid[2] = M1_KD;

    MOTOR_init(&M1,M1_EN,M1_DR);

    MOTOR_setPinDIR(&M1,0);
    MOTOR_setPinENABLE(&M1,0);
}

void initMotor2()
{
    // Initialize hardware related to motor M2
    PWM_M2_Start();
    DC_M2_Start();

    // initialize software associated to motor 2M2
    PIN_t M2_DR,M2_EN;
    M2_DR.DR = &M2_DIR_DR;
    M2_DR.MASK = M2_DIR_MASK;
    M2_DR.STATE = 0;
    M2_EN.DR = &M2_EN_DR;
    M2_EN.MASK = M2_EN_MASK;
    M2_EN.STATE = 0;

    M2.init_pos = 0;
    M2.control_mode = 1;
    M2.idx = I_MF_IDX;         // motor index
    DC_M2_SetCounter(M2.init_pos);

    M2.rvt_pid[0] = M1_KP;    // kp
    M2.rvt_pid[1] = M1_KI;   // kI
    M2.rvt_pid[2] = M1_KD;   // kD
       
    MOTOR_init(&M2,M2_EN,M2_DR);

    MOTOR_setPinDIR(&M2,0);
    MOTOR_setPinENABLE(&M2,0);

}

void initMotor3()
{
    // Initialize hardware related to motor M2
    PWM_M3_Start();
    DC_M3_Start();

    // initialize software associated to motor 2M2
    PIN_t M3_DR,M3_EN;
    M3_DR.DR = &M3_DIR_DR;
    M3_DR.MASK = M3_DIR_MASK;
    M3_DR.STATE = 0;
    M3_EN.DR = &M3_EN_DR;
    M3_EN.MASK = M3_EN_MASK;
    M3_EN.STATE = 0;

    M3.init_pos = 0;
    M3.control_mode = 1;
    M3.idx = P_ME_IDX;         // motor index
    DC_M3_SetCounter(M3.init_pos);

    M3.rvt_pid[0] = M1_KP;    // kp
    M3.rvt_pid[1] = M1_KI;   // kI
    M3.rvt_pid[2] = M1_KD;   // kD
       
    MOTOR_init(&M3,M3_EN,M3_DR);

    MOTOR_setPinDIR(&M3,0);
    MOTOR_setPinENABLE(&M3,0);

}

void initMotor4()
{
    // Initialize hardware related to motor M2
    PWM_M4_Start();
    DC_M4_Start();

    // initialize software associated to motor 2M2
    PIN_t M4_DR,M4_EN;
    M4_DR.DR = &M4_DIR_DR;
    M4_DR.MASK = M4_DIR_MASK;
    M4_DR.STATE = 0;
    M4_EN.DR = &M4_EN_DR;
    M4_EN.MASK = M4_EN_MASK;
    M4_EN.STATE = 0;

    M4.init_pos = 0;
    M4.control_mode = 1;
    M4.idx = P_MF_IDX;         // motor index
    DC_M4_SetCounter(M4.init_pos);

    M4.rvt_pid[0] = M1_KP;    // kp
    M4.rvt_pid[1] = M1_KI;   // kI
    M4.rvt_pid[2] = M1_KD;   // kD
       
    MOTOR_init(&M4,M4_EN,M4_DR);

    MOTOR_setPinDIR(&M4,0);
    MOTOR_setPinENABLE(&M4,0);

}

void initMotor5()
{
    // Initialize hardware related to motor M2
    DAC_M5_Start();
    DC_M5_Start();
    
    // initialize software associated to motor 2M2
    PIN_t M5_DR,M5_EN;
    M5_DR.DR = &M5_DIR_DR;
    M5_DR.MASK = M5_DIR_MASK;
    M5_DR.STATE = 0;
    M5_EN.DR = &M5_EN_DR;
    M5_EN.MASK = M5_EN_MASK;
    M5_EN.STATE = 0;

    M5.init_pos = 0;
    M5.control_mode = 1;
    M5.idx = M_ME_IDX;         // motor index
    DC_M5_SetCounter(M5.init_pos);

    M5.rvt_pid[0] = M2_KP;    // kp
    M5.rvt_pid[1] = M2_KI;   // kI
    M5.rvt_pid[2] = M2_KD;   // kD
       
    MOTOR_init(&M5,M5_EN,M5_DR);

    MOTOR_setPinDIR(&M5,0);
    MOTOR_setPinENABLE(&M5,0);

}

void initMotor6()
{
    // Initialize hardware related to motor M2
    DAC_M6_Start();

    // initialize software associated to motor 2M2
    PIN_t M6_DR,M6_EN;
    M6_DR.DR = &M6_DIR_DR;
    M6_DR.MASK = M6_DIR_MASK;
    M6_DR.STATE = 0;
    M6_EN.DR = &M6_EN_DR;
    M6_EN.MASK = M6_EN_MASK;
    M6_EN.STATE = 0;

    M6.init_pos = 0;
    M6.control_mode = 1;
    M6.idx = M_MF_IDX;         // motor index

    M6.counter = M6.init_pos;

    M6.rvt_pid[0] = M2_KP;    // kp
    M6.rvt_pid[1] = M2_KI;   // kI
    M6.rvt_pid[2] = M2_KD;   // kD
       
    MOTOR_init(&M6,M6_EN,M6_DR);

    MOTOR_setPinDIR(&M6,0);
    MOTOR_setPinENABLE(&M6,0);

}

void initHW()
{
    millis_Start();

    AMux_Start();
    
    SENSOR_ADC_Start();    
    UART_Start();

    /* Initialize general interrupt blocks */
    RxInt_StartEx(MyRxInt);
    spd_m2_StartEx(SPD_M2_INT);
    M6_HA_StartEx(M6_HA_INT);
    
}

void initHand()
{
    initMotor1();
    initMotor2();
    initMotor3();
    initMotor4();
    initMotor5();
    initMotor6();
     
    motors[I_MF_IDX] = &I_MF;
    motors[I_ME_IDX] = &I_ME;
    motors[P_MF_IDX] = &P_MF;
    motors[P_ME_IDX] = &P_ME;
    motors[M_MF_IDX] = &M_MF;
    motors[M_ME_IDX] = &M_ME;
    
    indice.tension_pid[0] = F1_T_KP;
    indice.tension_pid[1] = F1_T_KI;
    indice.tension_pid[2] = F1_T_KD;
    
    FINGER_init(&indice,&I_ME,&I_MF);
    FINGER_init(&pulgar,&P_ME,&P_MF);
    FINGER_init(&medio,&M_ME,&M_MF);
    
    fingers[P_IDX] = &pulgar;
    fingers[I_IDX] = &indice;
    fingers[M_IDX] = &medio;
    
    pulgar.idx = P_IDX;
    indice.idx = I_IDX;
    medio.idx = M_IDX;
}

int main(void)
{
    _state_ = 0;
    
    // initialize general hardware
    initHW();
    // initialize motors hardware and fingers structures
    initHand();
    
    // disable all motors
    for(int i=0; i<NUM_MOTORS; i++){
        MOTOR_Enable(motors[i]);
    }
    
    /* Enable global interrupts. */
    CyGlobalIntEnable;

    ContinuouslySendData = FALSE;

    /* control rates */
    uint16_t led_blink_rate = 1000/LED_BLINK_RATE;
    uint16_t rate_ms = 1000/RATE_HZ; // transmission rate
    uint16_t rvt_rate = 1000/RVT_RATE_HZ; // position control rate
    uint16_t tension_rate = 1000/TENSION_RATE_HZ; // position control rate

    uint32_t led_time = millis_ReadCounter();
    uint32_t actual_time = millis_ReadCounter();
    uint32_t rvt_time = millis_ReadCounter();
    uint32_t tension_time = millis_ReadCounter();

    uint16_t fs1 = 0, fs2 = 0, fs3 = 0, pote = 0;
    sumFs1 = 0;
    sumFs2 = 0;
    sumFs3 = 0;
    sumPote = 0;
    
    for(;;) // main loop
    {
        // lee sensores de presion
        read_smooth(100);
        indice.pressure_sensor = sumFs2; // -90 por un error acumulado del "smooth"    
        pulgar.pressure_sensor = sumFs1;
        medio.pressure_sensor = sumFs3;
        pote = sumPote; 
        
        // receive uart data
        while(IsCharReady()){
            //UART_PutString("&IsCharReady\r\n");
            if(GetRxStr()){
                //UART_PutString("&GetRxStr\r\n");
                ProcessCommandMsg();
            }
        }
        
        if(!SW1_Read()){
            for(int i=0; i<NUM_MOTORS; i++){
                MOTOR_setZeroPosistion(motors[i]);
            }/*
            while(!SW1_Read()){
                for(int i=0; i<NUM_MOTORS; i++){
                    MOTOR_setZeroPosistionManual(motors[i]);
                }
            }*/
        }

        if(millis_ReadCounter() - led_time > led_blink_rate)
        {
            _state_ = !_state_;
            LED1_Write(_state_);
            led_time = millis_ReadCounter();
        }

        // main counter loop
        if(millis_ReadCounter() - actual_time > rate_ms)
        {
            if(actual_time > 600000)
            {
                millis_WriteCounter(0); //resetea el contador cuando pasa los 600 segundos
                //actual_time = millis_ReadCounter();
            }
            MOTOR_readCurrentRvt(motors[5]);
            /* Send data based on last UART command */
            if(ContinuouslySendData)
            {
                int len = sizeof(WB.buffStr)/sizeof(*WB.buffStr);
                WB.cmd = F_UPDATE_PLOT;
                WB.motor = RB.id;
                
                WB.ref = motors[RB.id]->ref_rvt;
                WB.cur = motors[RB.id]->curr_rvt;
                WB.val = motors[RB.id]->applied_pwm;
    
                UART_PutArray((const uint8*)&WB.buffStr,len);
            }
            actual_time = millis_ReadCounter();
        }

        // position control loop
        if(millis_ReadCounter() - rvt_time > rvt_rate) {
            
            // revisa posicion actual y corrije
            for(int i=0; i<NUM_MOTORS; i++){
                MOTOR_readCurrentRvt(motors[i]);
                MOTOR_setPosition(motors[i]);
            }
            rvt_time = millis_ReadCounter();
        }
        
        if(millis_ReadCounter() - tension_time > tension_rate) {
            //echomsg(motors[1]->rvt_controller.kP ,motors[1]->ref_rvt,motors[1]->rvtPID_out,motors[1]->curr_rvt);
            FINGER_readTension(&indice);
            
            if(indice.ref_tension > _FINGER_MAX_TENSION){
                FINGER_setTensionRef(&indice,_FINGER_MAX_TENSION);   
            }
            else if(indice.ref_tension < _FINGER_MIN_TENSION){
                FINGER_setTensionRef(&indice,_FINGER_MIN_TENSION);   
            }
            FINGER_setTension(&indice);
            tension_time = millis_ReadCounter();
        }
        
    }
}

/* [] END OF FILE */
