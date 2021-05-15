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

void echomsg(uint8_t cmd,int16_t ref, int16_t cur, int16_t val)
{
    int len = sizeof(WB.buffStr)/sizeof(*WB.buffStr);
    WB.xff = FF;
    WB.cmd = cmd;
    WB.ref = ref;
    WB.cur = cur;
    WB.val = val;

    UART_PutArray((const uint8*)&WB.buffStr,len);
}

void sendPIDdata(int id)
{
    uint8_t cmd = 5; //send pid data
    int16_t ref = motors[id]->rvt_controller.kP;
    int16_t cur = motors[id]->rvt_controller.kI;
    int16_t val = motors[id]->rvt_controller.kD;    

    echomsg(cmd,ref,cur,val);
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

int16_t smooth(const int sensor, int numRead){
  int16_t i;
  int16_t numReadings = numRead;

//  for (i = 0; i < numReadings; i++){
    // Read light sensor data.
    switch(sensor)
    {
        case 0:
            sumFS1 = sumFS1 + (FORCE_SENSOR_1_ADC_GetResult16()-sumFS1)/numReadings;
            break;
        case 1:
            break;
    }   
    // 1ms pause adds more stability between reads.
    // CyDelay(1);
//  }

  // Take an average of all the readings.
  //sumFS1 = sumFS1 / numReadings;

  // Scale to 8 bits (0 - 255).
  // value = value / 4;

  return sumFS1;
}

uint16_t getTension(uint16_t read)
{
    // returns A101 sensor reading in g
    uint16_t tension = 0;
    float c_factor = 0.2908;
    float bit_factor = 4096.0;
    float force = read * 5.0/bit_factor*c_factor; 
    
    // using the ecuation for equilibrum state
    float Df = 0.017; // distance in meters
    float de = 0.0085;
    float alpha = atan(0.003/de) * 3.14159 / 180; // in radians
    float beta = atan(0.003/Df) * 3.14159 / 180; // in radians

    tension = Df*force*cos(alpha) / (de * cos(beta))*1000;

    return tension;
}

void ProcessCommandMsg(void)
{
    float p,i,d;
 
    switch(RB.cmd)
    {
        case 0: /* set reference of speed controller of motor RB.id */
            motors[RB.id]->ref_spd = RB.pref;
            break;
        case F_SET_POS_REF: /* set reference of position controller of motor RB.id */
            MOTOR_setRvtRef(motors[RB.id],RB.pref);
            //echomsg(1,0,0,RB.pref);
            break;
        case 2: /* set reference of tension controller of motor RB.id */
        
            break;
        case F_SEND_DATA_FALSE: /* Stop data streaming */
            ContinuouslySendData = FALSE;
            break;
        case F_REQ_PID_VALUES: /* request pid values */
            sendPIDdata(RB.id); // send controller parameters
            break;
        case F_SET_CONTROL_MODE: /* set control mode */
            // cmd(0,24,0,0,0,0) speed
            // cmd(0,24,1,0,0,0) position
            MOTOR_setControlMode(motors[RB.id],RB.pref);  
            break;
        case F_DISABLE_MOTOR: /* stop motor */
            if(motors[RB.id]->ENABLE.STATE == 1){
                MOTOR_setPinENABLE(motors[RB.id], 0);
            }
            break;
        case F_SET_PID_VALUES: /* Set pid values */ 
            
            p = ((float)RB.P)/FLOAT_TO_INT_SCALE;
            i = ((float)RB.I)/FLOAT_TO_INT_SCALE;
            d = ((float)RB.D)/FLOAT_TO_INT_SCALE;
            
            //echof(p);
            
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
            break;
        case F_SEND_DATA_TRUE: /* Contuously send data */
            ContinuouslySendData = TRUE;
            break;
        case F_ENABLE_MOTOR: /* resume motor */
            if(motors[RB.id]->ENABLE.STATE == 0){
                MOTOR_setPinENABLE(motors[RB.id], 1);
            }
            break;
        case 55: /* debug variables */
            sprintf(strMsg,"%d\t%d",(int)motors[0]->rvt_controller.inputValue,(int)(motors[0]->rvt_controller.kP*factor));
            UART_PutString(strMsg); 
            break;
        case 99: /* sw reset */
            CySoftwareReset();
            break;         
    }
    
}

/* [] END OF FILE */
