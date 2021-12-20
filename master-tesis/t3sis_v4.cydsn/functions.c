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

#include <functions.h>
#include <stdlib.h>

void echo(char* data)
{
    sprintf(strMsg,"*%s\r\n", data);
    UART_PutString(strMsg);
}
void echod(int data)
{
    sprintf(strMsg,"*%d\r\n", data);
    UART_PutString(strMsg);
}

void echof(float data)
{
    sprintf(strMsg,"*%.4f\r\n", data);
    UART_PutString(strMsg);
}

void echomsg(uint8_t cmd, uint8_t motor, int16_t val1, int16_t val2, int16_t val3)
{
    int len = sizeof(WB.buffStr)/sizeof(*WB.buffStr);
    WB.cmd = cmd;
    WB.motor = motor;
    WB.ref = val1;
    WB.cur = val2;
    WB.val = val3;

    UART_PutArray((const uint8*)&WB.buffStr,len);
}

void sendPIDdata(int id)
{
    uint8_t cmd = F_GET_PID_VALUES; //send pid data
    uint8_t motor;
    int16_t kp,ki,kd; 
    if(id >= 0 && id < 6){
        motor = id;
        kp = motors[id]->rvt_controller.kP;
        ki = motors[id]->rvt_controller.kI;
        kd = motors[id]->rvt_controller.kD;
    }
    // fingers index
    // 6 finger 1
    // 7 finger 2
    // 8 finger 3
    else if (id >= 6 && id < 9){
        motor = id;
        kp = fingers[id-6]->ten_controller.kP;
        ki = fingers[id-6]->ten_controller.kI;
        kd = fingers[id-6]->ten_controller.kD;
    }
    
    echomsg(cmd,id,kp,ki,kd);
}

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{    
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (int32_t)(out_min + slope*(x - in_min));   
}

uint8_t fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, uint8_t out_min, uint8_t out_max)
{
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (out_min + slope*(x - in_min));
}

void read_smooth(int numRead){
  uint16_t numReadings = numRead;
  uint16_t adc_reading[3] = {0,0,0};

  for(int i=0; i<=2; i++){
    AMux_FastSelect(i);
    SENSOR_ADC_StartConvert();
    SENSOR_ADC_IsEndConversion(SENSOR_ADC_WAIT_FOR_RESULT);
    adc_reading[i] = SENSOR_ADC_GetResult16();
    SENSOR_ADC_StopConvert();                
  }
  sumFs1 += (adc_reading[_FS1]- sumFs1)/numReadings;
  sumFs2 += (adc_reading[_FS2]- sumFs2)/numReadings;
  sumPote += (adc_reading[_POTE]- sumPote)/numReadings;

}

void ProcessCommandMsg(void)
{
    float p,i,d;
 
    switch(RB.cmd)
    {
        case F_FINGER_OPEN:
            FINGER_open(fingers[RB.id]);
            break;
        case F_FINGER_CLOSE:
            FINGER_close(fingers[RB.id]);
            break;
        case F_SET_SPEED_REF: /* set reference of speed controller of motor RB.id */
            motors[RB.id]->ref_spd = RB.pref;
            break;
        case F_SET_POS_REF: /* set reference of position controller of motor RB.id */
            MOTOR_setRvtRef(indice.M[RB.id],RB.pref);
            echomsg(F_SET_POS_REF,RB.id,0,0,RB.pref);
            break;
        case F_SET_FORCE_REF: /* set reference of tension controller of finger RB.id */
            FINGER_setTensionRef(fingers[RB.id-6],RB.pref);
            echomsg(F_SET_FORCE_REF,RB.id,0,0,fingers[RB.id]->ref_tension);
            break;
        case F_REQ_PID_VALUES: /* request pid values */
            //echomsg(F_REQ_PID_VALUES,RB.id,0,0,0);
            sendPIDdata(RB.id); // send controller parameters
            break;
        case F_SET_CONTROL_MODE: /* set control mode */
            // 0 speed
            // 1 revolution
            // 2 tension    
            //echomsg(F_SET_CONTROL_MODE,RB.id,0,0,0);
            if (RB.pref == REV_CTRL) MOTOR_setControlMode(motors[RB.id],RB.pref);  
            else if (RB.pref == TEN_CTRL) {
                MOTOR_setControlMode(indice.M[F1_ME_IDX],M_FORCE_CONTROL_MODE);  
                MOTOR_setControlMode(indice.M[F1_MF_IDX],M_POSITION_CONTROL_MODE);
                MOTOR_Enable(indice.M[F1_ME_IDX]);
                MOTOR_Enable(indice.M[F1_MF_IDX]);
            }
            break;
        case F_ENABLE_MOTOR: /* enable motor movement: args id,cmd */
            if(motors[RB.id]->ENABLE.STATE == 0){
                MOTOR_Enable(motors[RB.id]);
            }
            break;
        case F_DISABLE_MOTOR: /* disable motor movement: args id,cmd */
            if(motors[RB.id]->ENABLE.STATE == 1){
                MOTOR_Disable(motors[RB.id]);
            }
            break;
        case F_SET_PID_VALUES: /* Set pid values */ 
            
            p = ((float)RB.P)/FLOAT_TO_INT_SCALE;
            i = ((float)RB.I)/FLOAT_TO_INT_SCALE;
            d = ((float)RB.D)/FLOAT_TO_INT_SCALE;
            
            if(motors[RB.id]->control_mode == 0)
            {
                /*
                EEPROM_1_Write((const uint8*)&RB.P,(uint8)CYDEV_EE_BASE);
                EEPROM_1_Write((const uint8*)&RB.I,(uint8)CYDEV_EE_BASE+0x10);
                EEPROM_1_Write((const uint8*)&RB.D,(uint8)CYDEV_EE_BASE+0x20);
                */
                MOTOR_setSpdControlParams(motors[RB.id],p,i,d);
            }
            else if(motors[RB.id]->control_mode == 1)
            {
                /*
                EEPROM_1_Write((const uint8*)&p,3);
                EEPROM_1_Write((const uint8*)&i,4);
                EEPROM_1_Write((const uint8*)&d,5);
                */
                MOTOR_setRvtControlParams(motors[RB.id],p,i,d);   
            }
            else if(RB.id == 6)
            {
                /*
                EEPROM_1_Write((const uint8*)&p,3);
                EEPROM_1_Write((const uint8*)&i,4);
                EEPROM_1_Write((const uint8*)&d,5);
                */
                FINGER_setTensionControlParams(fingers[RB.id-6],p,i,d);
            }
            break;
        case F_SEND_DATA_FALSE: /* Stop data streaming: args cmd */
            ContinuouslySendData = FALSE;
            break;
        case F_SEND_DATA_TRUE: /* Contuously send data: args cmd */
            ContinuouslySendData = TRUE;
            break;
        case F_DEBUG_VAR: /* debug variables */
            sprintf(strMsg,"%d\t%d",(int)motors[0]->rvt_controller.inputValue,(int)(motors[0]->rvt_controller.kP*factor));
            UART_PutString(strMsg); 
            break;
        case F_SOFTWARE_RESET: /* sw reset */
            CySoftwareReset();
            break;         
    }
    
}

/* [] END OF FILE */
