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
    HA_TIMER_M1_Start();
    DC_M1_Start();
    HA_ISR_M1_StartEx(M1_HA_INT);

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
    M1.idx = M1_IDX;         // motor index
    DC_M1_SetCounter(M1.init_pos);

    M1.rvt_pid[0] = M1_KP;
    M1.rvt_pid[1] = M1_KI;
    M1.rvt_pid[2] = M1_KD;

    MOTOR_init(&M1,M1_EN,M1_DR);

    MOTOR_setPinDIR(&M1,1);
    MOTOR_setPinENABLE(&M1,1);
}

void initMotor2()
{
    // Initialize hardware related to motor M2
    PWM_M2_Start();
    HA_TIMER_M2_Start();
    DC_M2_Start();
    HA_ISR_M2_StartEx(M2_HA_INT);

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
    M2.idx = M2_IDX;         // motor index
    DC_M2_SetCounter(M2.init_pos);

    M2.rvt_pid[0] = M2_KP;    // kp
    M2.rvt_pid[1] = M2_KI;   // kI
    M2.rvt_pid[2] = M2_KD;   // kD
    
    MOTOR_init(&M2,M2_EN,M2_DR);

    MOTOR_setPinDIR(&M2,0);
    MOTOR_setPinENABLE(&M2,1);

}

void initMotors()
{
    initMotor1();
    initMotor2();

    motors[0] = &M1;
    motors[1] = &M2;
}

int main(void)
{
    millis_Start();

    AMux_Start();
    
    SENSOR_ADC_Start();    
    UART_Start();
    EEPROM_1_Start();

    /* Initialize general interrupt blocks */
    RxInt_StartEx(MyRxInt);
    spd_m2_StartEx(SPD_M2_INT);

    _state_ = 0;
    initMotors();

    CyGlobalIntEnable; /* Enable global interrupts. */

    ContinuouslySendData = FALSE;

    /* control rates */
    uint16_t led_blink_rate = 1000/LED_BLINK_RATE;
    uint16_t rate_ms = 1000/RATE_HZ; // transmission rate
    uint16_t rvt_rate = 1000/RVT_RATE_HZ; // position control rate

    uint32_t led_time = millis_ReadCounter();
    uint32_t actual_time = millis_ReadCounter();
    uint32_t rvt_time = millis_ReadCounter();

    uint16_t fs1 = 0, fs2 = 0, FS1 = 0, pote = 0;
    sumFs1 = 0;
    sumFs2 = 0;
    sumPote = 0;
    
    MOTOR_setPinENABLE(motors[1],M_ENABLE);
    MOTOR_setPinDIR(motors[1],M_CCW);

    for(;;) // main loop
    {
        read_smooth(5);
        
        // receive uart data
        while(IsCharReady()){
            //UART_PutString("&IsCharReady\r\n");
            if(GetRxStr()){
                //UART_PutString("&GetRxStr\r\n");
                ProcessCommandMsg();
            }
        }

        pote = sumPote;
        fs1 = sumFs1;
        fs2 = sumFs2;
        FS1 = getTension(fs1);
    
        // configura referencia de control
        //MOTOR_setRvtRef(motors[0],fs1/4);
        //MOTOR_setRvtRef(motors[1],fs2/4);
        
        //PWM_M1_WriteCompare(fn_mapper(fs1,0,4095,0,1200));
        //PWM_M2_WriteCompare(pote);

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

            /* Send data based on last UART command */
            if(ContinuouslySendData)
            {
                int len = sizeof(WB.buffStr)/sizeof(*WB.buffStr);
                WB.xff = FF;
                WB.cmd = M_PLOT_DATA_CMD;
                WB.ref = motors[RB.id]->ref_rvt;
                WB.cur = motors[RB.id]->curr_rvt;
                WB.val = sumFs1;

                UART_PutArray((const uint8*)&WB.buffStr,len);
            }
            actual_time = millis_ReadCounter();
        }

        // position control loop
        if(millis_ReadCounter() - rvt_time > rvt_rate) {
            //echomsg(motors[1]->rvt_controller.kP ,motors[1]->ref_rvt,motors[1]->rvtPID_out,motors[1]->curr_rvt);
            
            // revisa error del controlador y corrije
            for(int i=0; i<NUM_MOTORS; i++){
                if(motors[i]->ref_rvt > _MOTOR_MAX_RVT){
                    MOTOR_setRvtRef(motors[i],_MOTOR_MAX_RVT);   
                }
                if(motors[i]->ref_rvt < _MOTOR_MIN_RVT){
                    MOTOR_setRvtRef(motors[i],_MOTOR_MIN_RVT);
                }
                MOTOR_setPosition(motors[i]);
            }

            rvt_time = millis_ReadCounter();
        }
    }
}

/* [] END OF FILE */
