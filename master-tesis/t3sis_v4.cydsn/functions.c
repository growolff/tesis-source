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
    sprintf(strMsg1,"%s\r\n", data);
    UART_PutString(strMsg1);   
}
void echod(int data)
{
    sprintf(strMsg1,"%d\r\n", data);
    UART_PutString(strMsg1);   
}

int32_t fn_mapper(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{    
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (int32_t)(out_min + slope*(x - in_min));   
}

uint8_t fn_mapper_8b(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    double slope = 1.0 * (out_max - out_min)/(in_max - in_min);
    return (uint8)(out_min + slope*(x - in_min));
}

void ProcessCommandMsg(void)
{
    echo("TEST");
    echo(RB.RxStr);
    echod(RB.id);
    echod(RB.cmd);
    echod(RB.pref);
    echod(RB.tref);
    
    if(RB.cmd == 1)
    {
        motors[RB.id]->ref_rvt = RB.pref;        
    }
    
}

/* [] END OF FILE */
