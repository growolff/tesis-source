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
#include "project.h"
#include "myUART.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#define TRANSMIT_BUFFER_SIZE 16
#define FALSE 0

char msg[TRANSMIT_BUFFER_SIZE];

#define forever 1
    //buffer to hold application settings
    typedef struct TParamBuffer{
        char* cmd;  //command
        int pVal,iVal,dVal;   //int param
    } ParamBuffer;  //settings
    
    volatile ParamBuffer PB;     //volatile struct TParamBuffer PB;

void ProcessCommandMsg(void);// Process received command (do something...)

void InitializeHardware(void){
    
    RxInt_StartEx(MyRxInt);
    
    UART_Start();    
    CyDelay(250);
    UART_PutString("&UART Test\r\n");
    
}

int main(void){

    InitializeHardware();
    CyGlobalIntEnable; /* Enable global interrupts. */

    int c=0;
    for(;;)
    {
        while(IsCharReady()){
            if(GetRxStr()){
                ProcessCommandMsg();
            }   
        }
        c++;
    }
}

void ProcessCommandMsg(void){    
    //check received message for any valid command and execute it if necessary or report old value
    //if command not recognized, then report error
    //todo: add check for valid conversion string->value
     
    //sprintf(strMsg1,"%s\r", RB.RxStr); UART_PutString(strMsg1);
   
    //todo: ther are problems if terminator is "\r\n"
    
    if (RB.cmd == 'P'){
        if(strlen(RB.valstr) > 0){
            PB.pVal = atoi(RB.valstr);
            sprintf(msg,"&%c: %s\r\n",RB.cmd,RB.valstr);
            UART_PutString(msg);
        }
    }
    if (RB.cmd == 'I'){
        if(strlen(RB.valstr) > 0){
            PB.iVal = atoi(RB.valstr);
            sprintf(msg,"&%c: %s\r\n",RB.cmd,RB.valstr);
            UART_PutString(msg);
        }
    }
    if (RB.cmd == 'D'){
        if(strlen(RB.valstr) > 0){
            PB.dVal = atoi(RB.valstr);
            sprintf(msg,"&%c: %s\r\n",RB.cmd,RB.valstr);
            UART_PutString(msg);
        }
    }
    /*
    if     (RB.cmd == 'R')//command 'R' received..
    {
        if (strlen(RB.valstr) > 0) PB.R = atoi(RB.valstr);//set new value, else report old 
        sprintf(strMsg1,"R=%d\r\n", PB.R); UART_PutString(strMsg1);//echo command and value
        PinRED_Write(!PB.R);//swithch RED LED
    }
    else if (RB.cmd == 'G')//command 'R' received..
    {
        if (strlen(RB.valstr) > 0) PB.G = atoi(RB.valstr);//set new value, else report old 
        sprintf(strMsg1,"G=%d\r\n", PB.G); UART_PutString(strMsg1);//echo command and value
        PinGRN_Write(!PB.G);//swithch GRN LED
    }
    else if (RB.cmd == 'B')//command 'R' received..
    {
        if (strlen(RB.valstr) > 0) PB.B = atoi(RB.valstr);//set new value, else report old 
        sprintf(strMsg1,"B=%d\r\n", PB.B); UART_PutString(strMsg1);//echo command and value
        PinBLU_Write(!PB.B);//swithch BLU LED
    }
    else if (RB.cmd == 'X')//command 'X' received..
    {
        sprintf(strMsg1,"X=%s\r\n", RB.valstr); UART_PutString(strMsg1);//echo command and value
        PB.R=0; PB.G=0; PB.B=0;
        PinRED_Write(!PB.R); PinGRN_Write(!PB.G); PinBLU_Write(!PB.B);//turn all LED on
    }
    else if (RB.cmd == 'S')//command 'S' received..
    {
        sprintf(strMsg1,"S=%s\r\n", RB.valstr); UART_PutString(strMsg1);//echo command and value
        PB.R=1; PB.G=1; PB.B=1;
        PinRED_Write(!PB.R); PinGRN_Write(!PB.G); PinBLU_Write(!PB.B);//turn all LED on
    }
    else //command unrecognized - echo unrecognized command
    {
        sprintf(strMsg1,"!%s\r\n", RB.RxStr); UART_PutString(strMsg1);
    }
    */
}
/* [] END OF FILE */
