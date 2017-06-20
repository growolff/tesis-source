/*******************************************************************************
* File Name: isr_HA.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <isr_HA.h>
#include "cyapicallbacks.h"

#if !defined(isr_HA__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START isr_HA_intc` */
#include <Timer_ha.h>
      
volatile int32_t t_ha;
volatile int32_t ca;
volatile int32_t ma; 

volatile int32_t HA_counter;
    
/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: isr_HA_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. This function disables the interrupt, 
*  sets the default interrupt vector, sets the priority from the value in the
*  Design Wide Resources Interrupt Editor, then enables the interrupt to the 
*  interrupt controller.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_HA_Start(void)
{
    /* For all we know the interrupt is active. */
    isr_HA_Disable();

    /* Set the ISR to point to the isr_HA Interrupt. */
    isr_HA_SetVector(&isr_HA_Interrupt);

    /* Set the priority. */
    isr_HA_SetPriority((uint8)isr_HA_INTC_PRIOR_NUMBER);

    /* Enable it. */
    isr_HA_Enable();
}


/*******************************************************************************
* Function Name: isr_HA_StartEx
********************************************************************************
*
* Summary:
*  Sets up the interrupt and enables it. This function disables the interrupt,
*  sets the interrupt vector based on the address passed in, sets the priority 
*  from the value in the Design Wide Resources Interrupt Editor, then enables 
*  the interrupt to the interrupt controller.
*  
*  When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*  used to provide consistent definition across compilers:
*  
*  Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*   Function prototype example:
*   CY_ISR_PROTO(MyISR);
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void isr_HA_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    isr_HA_Disable();

    /* Set the ISR to point to the isr_HA Interrupt. */
    isr_HA_SetVector(address);

    /* Set the priority. */
    isr_HA_SetPriority((uint8)isr_HA_INTC_PRIOR_NUMBER);

    /* Enable it. */
    isr_HA_Enable();
}


/*******************************************************************************
* Function Name: isr_HA_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_HA_Stop(void)
{
    /* Disable this interrupt. */
    isr_HA_Disable();

    /* Set the ISR to point to the passive one. */
    isr_HA_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: isr_HA_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for isr_HA.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(isr_HA_Interrupt)
{
    #ifdef isr_HA_INTERRUPT_INTERRUPT_CALLBACK
        isr_HA_Interrupt_InterruptCallback();
    #endif /* isr_HA_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START isr_HA_Interrupt` */
    
    HA_counter+=1;
    ca = Timer_ha_ReadCounter(); // read counter
    t_ha = -1*(ca+ma); //
    ma = ca;
    Timer_ha_WriteCounter(0); // reset the counter

    /* `#END` */
}


/*******************************************************************************
* Function Name: isr_HA_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling isr_HA_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use isr_HA_StartEx instead.
* 
*   When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*   used to provide consistent definition across compilers:
*
*   Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*
*   Function prototype example:
*     CY_ISR_PROTO(MyISR);
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void isr_HA_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)isr_HA__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: isr_HA_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress isr_HA_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)isr_HA__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: isr_HA_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling isr_HA_Start or isr_HA_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after isr_HA_Start or isr_HA_StartEx has been called. 
*   To set the initial priority for the component, use the Design-Wide Resources
*   Interrupt Editor.
*
*   Note This API has no effect on Non-maskable interrupt NMI).
*
* Parameters:
*   priority: Priority of the interrupt, 0 being the highest priority
*             PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*             PSoC 4: Priority is from 0 to 3.
*
* Return:
*   None
*
*******************************************************************************/
void isr_HA_SetPriority(uint8 priority)
{
    *isr_HA_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: isr_HA_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt, 0 being the highest priority
*    PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*    PSoC 4: Priority is from 0 to 3.
*
*******************************************************************************/
uint8 isr_HA_GetPriority(void)
{
    uint8 priority;


    priority = *isr_HA_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: isr_HA_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt to the interrupt controller. Do not call this function
*   unless ISR_Start() has been called or the functionality of the ISR_Start() 
*   function, which sets the vector and the priority, has been called.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_HA_Enable(void)
{
    /* Enable the general interrupt. */
    *isr_HA_INTC_SET_EN = isr_HA__INTC_MASK;
}


/*******************************************************************************
* Function Name: isr_HA_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 isr_HA_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*isr_HA_INTC_SET_EN & (uint32)isr_HA__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: isr_HA_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt in the interrupt controller.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_HA_Disable(void)
{
    /* Disable the general interrupt. */
    *isr_HA_INTC_CLR_EN = isr_HA__INTC_MASK;
}


/*******************************************************************************
* Function Name: isr_HA_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
* Side Effects:
*   If interrupts are enabled and the interrupt is set up properly, the ISR is
*   entered (depending on the priority of this interrupt and other pending 
*   interrupts).
*
*******************************************************************************/
void isr_HA_SetPending(void)
{
    *isr_HA_INTC_SET_PD = isr_HA__INTC_MASK;
}


/*******************************************************************************
* Function Name: isr_HA_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt in the interrupt controller.
*
*   Note Some interrupt sources are clear-on-read and require the block 
*   interrupt/status register to be read/cleared with the appropriate block API 
*   (GPIO, UART, and so on). Otherwise the ISR will continue to remain in 
*   pending state even though the interrupt itself is cleared using this API.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_HA_ClearPending(void)
{
    *isr_HA_INTC_CLR_PD = isr_HA__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
#if 0 /* begin disabled code */
`#start isr_1_intc` -- section removed from template

#include <Timer_1.h>
      
volatile int32_t _vel_counter;
volatile int32_t debug;
volatile int32_t _last_vel;
volatile int32_t _time_btwn_re;
int cb=0,mb=0,t_hb=0;
    
`#end`

#endif /* end disabled code */
#if 0 /* begin disabled code */
`#start isr_1_Interrupt` -- section removed from template

    int32_t sum=0;
    int32_t _vel_counter_filtered=0;
    int32_t _vel = 0;
    int i,j,a;

    //_vel_counter = Vel_counter_ReadCapture();
    
    _vel_counter = Timer_1_ReadCounter();
    
    _time_btwn_re = _vel_counter - _last_vel;
    
    _last_vel = _vel_counter;
    
    /*
    switch(state){
        case 0: // fill initial array
            _last[idx] = _vel_counter;
            idx++;
            state = idx==_mean_window ? 1 : 0;
            break;
        case 1:
            // apply median filter to discard outliers
            // sort the array
            for(i=0;i<_mean_window;++i){
                for(j=i+1;j<_mean_window;++j){
                    if(_last[i] > _last[j]){
                        a=_last[i];
                        _last[i]=_last[j];
                        _last[j]=a;
                    }
                }
            }
            _last_median = _last[_mean_window/2];
            
            _last[idx%_mean_window] = (_vel_counter > _last_median*1.5 || _vel_counter < _last_median*0.5) ? _last_median : _vel_counter ;

            idx++;
            
            _vel = 60/(4*_last[4]*0.0001);
            debug = _last_median;

    }   
    */
    // Vel_counter_Init();
    
`#end`

#endif /* end disabled code */
