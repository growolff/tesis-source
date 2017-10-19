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
#include <math.h>

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{    
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (int32_t)(out_min + slope*(x - in_min));   
}

uint8 fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (uint8)(out_min + slope*(x - in_min));
}

double Sigmoid(double x,double a, double b)
{
    return a*(2/(1 + exp(-x*b))-1);
}

void ProcessCommandMsg(void)
{    
    //check received message for any valid command and execute it if necessary or report old value
    //if command not recognized, then report error
    //todo: add check for valid conversion string->value
     
    //sprintf(strMsg1,"%s\r", RB.RxStr); UART_PutString(strMsg1);
   
    //todo: ther are problems if terminator is "\r\n"
    uint8 updatePID = FALSE;
    
    if (RB.cmd == 'P'){
        if(strlen(RB.valstr) > 0){
            updatePID = TRUE;
            PB.pVal = atoi(RB.valstr);
            //sprintf(msg,"&%c: %s\r\n",RB.cmd,RB.valstr);
            //UART_PutString(msg);
        }
    }
    else if (RB.cmd == 'I'){
        if(strlen(RB.valstr) > 0){
            updatePID = TRUE;
            PB.iVal = atoi(RB.valstr);
            //sprintf(msg,"&%c: %s\r\n",RB.cmd,RB.valstr);
            //UART_PutString(msg);
        }
    }
    else if (RB.cmd == 'D'){
        if(strlen(RB.valstr) > 0){
            updatePID = TRUE;
            PB.dVal = atoi(RB.valstr);
            //sprintf(msg,"&%c: %s\r\n",RB.cmd,RB.valstr);
            //UART_PutString(msg);
        }
    }
    else if (RB.cmd == 's'){
        //UART_PutString("&S");
        ContinuouslySendData = TRUE;
    }
    else if (RB.cmd == 'q'){
        CySoftwareReset();
    }
    else if (RB.cmd == 'x'){
        //UART_PutString("&X");
        ContinuouslySendData = FALSE;
    }
    else if (RB.cmd == 'b'){
        HandBrake();
    }
    else if (RB.cmd == '!'){
        if(strlen(RB.valstr) > 0){
            pos_ref = atoi(RB.valstr);
        }
    }
    else if (RB.cmd == '#'){
        if(strlen(RB.valstr) > 0){
            speed_ref = atoi(RB.valstr);
        }
    }
    else if (RB.cmd == '$'){
        if(strlen(RB.valstr) > 0){
            tension_ref = atoi(RB.valstr);
        }
    }
    else if (RB.cmd == '1'){
        pos_ref = 20;
    }
    else if (RB.cmd == '0'){
        pos_ref = 0;
    }
    
    if ( updatePID == TRUE ){
        PID_setCoeffs(&speed_pid_,(float)PB.pVal/100.0,(float)PB.iVal/100.0,(float)PB.dVal/100.0);
        updatePID = FALSE;
    }
}

void HandBrake()
{
    if (_braken_state == 0){
        BRAKEn_Write(1);
        _braken_state = 1;
    }
    else{
        BRAKEn_Write(0);
        _braken_state = 0;
    }
}

int16 * StoreResults() // store ADC conversion result in a sensor data array
{
	uint16 i;
    static int16 dest[NUM_SENSORS];
	
	for (i = 0; i < NUM_SENSORS; i++) {
		dest[i] = ADC_TS_GetResult16(i);
	}
    return dest;
}

/* [] END OF FILE */
