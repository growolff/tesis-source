/*******************************************************************************
* File Name: DirCounter.c  
* Version 3.0
*
* Description:
*  This file provides the source code to the API for the Quadrature Decoder
*  component.
*
* Note:
*  None.
*   
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "DirCounter.h"

#if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
    #include "DirCounter_PVT.h"
#endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT */

uint8 DirCounter_initVar = 0u;


/*******************************************************************************
* Function Name: DirCounter_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default QuadDec configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DirCounter_Init(void) 
{
    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(DirCounter_ISR_NUMBER);
        /* Set the ISR to point to the DirCounter_isr Interrupt. */
        (void) CyIntSetVector(DirCounter_ISR_NUMBER, & DirCounter_ISR);
        /* Set the priority. */
        CyIntSetPriority(DirCounter_ISR_NUMBER, DirCounter_ISR_PRIORITY);
    #endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: DirCounter_Enable
********************************************************************************
*
* Summary:
*  This function enable interrupts from Component and also enable Component's
*  ISR in case of 32-bit counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DirCounter_Enable(void) 
{
    uint8 enableInterrupts;

    DirCounter_SetInterruptMask(DirCounter_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) DirCounter_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    DirCounter_SR_AUX_CONTROL |= DirCounter_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(DirCounter_ISR_NUMBER);
    #endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: DirCounter_Start
********************************************************************************
*
* Summary:
*  Initializes UDBs and other relevant hardware.
*  Resets counter, enables or disables all relevant interrupts.
*  Starts monitoring the inputs and counting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  DirCounter_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DirCounter_Start(void) 
{
    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT)
        DirCounter_Cnt8_Start();
        DirCounter_Cnt8_WriteCounter(DirCounter_COUNTER_INIT_VALUE);
    #else
        /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT) || 
        *  (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT) 
        */
        DirCounter_Cnt16_Start();
        DirCounter_Cnt16_WriteCounter(DirCounter_COUNTER_INIT_VALUE);
    #endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT */
    
    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)        
       DirCounter_count32SoftPart = 0;
    #endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT */

    if (DirCounter_initVar == 0u)
    {
        DirCounter_Init();
        DirCounter_initVar = 1u;
    }

    DirCounter_Enable();
}


/*******************************************************************************
* Function Name: DirCounter_Stop
********************************************************************************
*
* Summary:
*  Turns off UDBs and other relevant hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DirCounter_Stop(void) 
{
    uint8 enableInterrupts;

    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT)
        DirCounter_Cnt8_Stop();
    #else 
        /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT) ||
        *  (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
        */
        DirCounter_Cnt16_Stop();    /* counter disable */
    #endif /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    DirCounter_SR_AUX_CONTROL &= (uint8) (~DirCounter_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
        CyIntDisable(DirCounter_ISR_NUMBER);    /* interrupt disable */
    #endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: DirCounter_GetCounter
********************************************************************************
*
* Summary:
*  Reports the current value of the counter.
*
* Parameters:
*  None.
*
* Return:
*  The counter value. Return type is signed and per the counter size setting.
*  A positive value indicates clockwise movement (B before A).
*
* Global variables:
*  DirCounter_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int16 DirCounter_GetCounter(void) 
{
    int16 count;
    uint16 tmpCnt;

    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(DirCounter_ISR_NUMBER);

        tmpCnt = DirCounter_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) DirCounter_COUNTER_INIT_VALUE);
        count = DirCounter_count32SoftPart + hwCount;

        CyIntEnable(DirCounter_ISR_NUMBER);
    #else 
        /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT) || 
        *  (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT)
        */
        #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT)
            tmpCnt = DirCounter_Cnt8_ReadCounter();
        #else /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT) */
            tmpCnt = DirCounter_Cnt16_ReadCounter();
        #endif  /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT */

        count = (int16) ((int32) tmpCnt -
                (int32) DirCounter_COUNTER_INIT_VALUE);

    #endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: DirCounter_SetCounter
********************************************************************************
*
* Summary:
*  Sets the current value of the counter.
*
* Parameters:
*  value:  The new value. Parameter type is signed and per the counter size
*  setting.
*
* Return:
*  None.
*
* Global variables:
*  DirCounter_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DirCounter_SetCounter(int16 value) 
{
    #if ((DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT) || \
         (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + DirCounter_COUNTER_INIT_VALUE;
        }
        else
        {
            count = DirCounter_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT)
            DirCounter_Cnt8_WriteCounter(count);
        #else /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT) */
            DirCounter_Cnt16_WriteCounter(count);
        #endif  /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT */
    #else /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT) */
        CyIntDisable(DirCounter_ISR_NUMBER);

        DirCounter_Cnt16_WriteCounter(DirCounter_COUNTER_INIT_VALUE);
        DirCounter_count32SoftPart = value;

        CyIntEnable(DirCounter_ISR_NUMBER);
    #endif  /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT) ||
             * (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: DirCounter_GetEvents
********************************************************************************
* 
* Summary:
*   Reports the current status of events. This function clears the bits of the 
*   status register.
*
* Parameters:
*  None.
*
* Return:
*  The events, as bits in an unsigned 8-bit value:
*    Bit      Description
*     0        Counter overflow.
*     1        Counter underflow.
*     2        Counter reset due to index, if index input is used.
*     3        Invalid A, B inputs state transition.
*
*******************************************************************************/
uint8 DirCounter_GetEvents(void) 
{
    return (DirCounter_STATUS_REG & DirCounter_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: DirCounter_SetInterruptMask
********************************************************************************
*
* Summary:
*  Enables / disables interrupts due to the events.
*  For the 32-bit counter, the overflow, underflow and reset interrupts cannot
*  be disabled, these bits are ignored.
*
* Parameters:
*  mask: Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*
* Return:
*  None.
*
*******************************************************************************/
void DirCounter_SetInterruptMask(uint8 mask) 
{
    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (DirCounter_COUNTER_OVERFLOW | DirCounter_COUNTER_UNDERFLOW |
                 DirCounter_COUNTER_RESET);
    #endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT */

    DirCounter_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: DirCounter_GetInterruptMask
********************************************************************************
*
* Summary:
*  Reports the current interrupt mask settings.
*
* Parameters:
*  None.
*
* Return:
*  Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*  For the 32-bit counter, the overflow, underflow and reset enable bits are
*  always set.
*
*******************************************************************************/
uint8 DirCounter_GetInterruptMask(void) 
{
    return (DirCounter_STATUS_MASK & DirCounter_INIT_INT_MASK);
}


/* [] END OF FILE */
