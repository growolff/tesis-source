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

#ifndef MY_UART_FILE_H
#define MY_UART_FILE_H
    
#include <project.h>            

#define RxBufferSize	128     //size of the Rx circular buffer
//any standard terminator (or their combination) will work
#define EOM_CR        0x0D    //message separator char (\r)
#define EOM_LF        0x0A    //message separator char (\n)
#define HASH          0x23
  
// each Rx command consists of: <char command><string value><CR>

union TRxBuffer {
    char   RxStr[18];   // received Rx packet (to hold individual command message)
    struct {            // anonimous structure
        uint8_t xff;
        uint8_t xff1;
        uint8_t id;      // motor id
        uint8_t cmd;     // 1-byte command: Write, Read, Control, get info, set pid, etc.
        int16_t pref;    // position reference
        int16_t tref;
        int16_t P;      // 
        int16_t I;
        int16_t D;
    };
} RB;
   
//=====================================
//        Function Prototypes 
//=====================================
   
CY_ISR_PROTO(MyRxInt);          // process Rx interrupt

uint8	IsCharReady(void);		// Returns TRUE when a char is ready
uint8	GetRxStr(void);			// Extract command string from buffer, non-blocking 

#endif // MY_UART_FILE_H

/* [] END OF FILE */
