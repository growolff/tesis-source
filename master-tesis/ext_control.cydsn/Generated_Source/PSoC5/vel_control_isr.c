/*******************************************************************************
* File Name: vel_control_isr.c  
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
#include <vel_control_isr.h>
#include "cyapicallbacks.h"

#if !defined(vel_control_isr__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START vel_control_isr_intc` */

#include <PWM.h>
#include "global.h"

volatile int32_t t_ha;
volatile int32_t _pid_out;
volatile int32_t speed_ref;
volatile int32_t current_speed;
volatile uint8 _pVal;
    
PID_t speed_pid_;
int32_t speed_pid_output;
uint8 _pid_pwm_out;

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
* Function Name: vel_control_isr_Start
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
void vel_control_isr_Start(void)
{
    /* For all we know the interrupt is active. */
    vel_control_isr_Disable();

    /* Set the ISR to point to the vel_control_isr Interrupt. */
    vel_control_isr_SetVector(&vel_control_isr_Interrupt);

    /* Set the priority. */
    vel_control_isr_SetPriority((uint8)vel_control_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    vel_control_isr_Enable();
}


/*******************************************************************************
* Function Name: vel_control_isr_StartEx
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
void vel_control_isr_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    vel_control_isr_Disable();

    /* Set the ISR to point to the vel_control_isr Interrupt. */
    vel_control_isr_SetVector(address);

    /* Set the priority. */
    vel_control_isr_SetPriority((uint8)vel_control_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    vel_control_isr_Enable();
}


/*******************************************************************************
* Function Name: vel_control_isr_Stop
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
void vel_control_isr_Stop(void)
{
    /* Disable this interrupt. */
    vel_control_isr_Disable();

    /* Set the ISR to point to the passive one. */
    vel_control_isr_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: vel_control_isr_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for vel_control_isr.
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
CY_ISR(vel_control_isr_Interrupt)
{
    #ifdef vel_control_isr_INTERRUPT_INTERRUPT_CALLBACK
        vel_control_isr_Interrupt_InterruptCallback();
    #endif /* vel_control_isr_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START vel_control_isr_Interrupt` */

    current_speed = HF_CLK/t_ha * 30; // 60 * 2 / 4 = 30, in RPM
    
    PID_setRef(&speed_pid_,speed_ref);
    
    if(current_speed == 0){
        speed_pid_.iTerm = 0;
    }
    speed_pid_output = PID_calculatePID(&speed_pid_,current_speed);
    
    // maps the pid_output to a 8bit number for pwm control of motor
    _pid_pwm_out = fn_mapper_8b(speed_pid_output,0,10000,0,255);
    debug = _pid_pwm_out;
    PWM_WriteCompare((uint8)(255-_pid_pwm_out));
    
    /* `#END` */
}


/*******************************************************************************
* Function Name: vel_control_isr_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling vel_control_isr_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use vel_control_isr_StartEx instead.
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
void vel_control_isr_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)vel_control_isr__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: vel_control_isr_GetVector
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
cyisraddress vel_control_isr_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)vel_control_isr__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: vel_control_isr_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling vel_control_isr_Start or vel_control_isr_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after vel_control_isr_Start or vel_control_isr_StartEx has been called. 
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
void vel_control_isr_SetPriority(uint8 priority)
{
    *vel_control_isr_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: vel_control_isr_GetPriority
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
uint8 vel_control_isr_GetPriority(void)
{
    uint8 priority;


    priority = *vel_control_isr_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: vel_control_isr_Enable
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
void vel_control_isr_Enable(void)
{
    /* Enable the general interrupt. */
    *vel_control_isr_INTC_SET_EN = vel_control_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: vel_control_isr_GetState
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
uint8 vel_control_isr_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*vel_control_isr_INTC_SET_EN & (uint32)vel_control_isr__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: vel_control_isr_Disable
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
void vel_control_isr_Disable(void)
{
    /* Disable the general interrupt. */
    *vel_control_isr_INTC_CLR_EN = vel_control_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: vel_control_isr_SetPending
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
void vel_control_isr_SetPending(void)
{
    *vel_control_isr_INTC_SET_PD = vel_control_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: vel_control_isr_ClearPending
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
void vel_control_isr_ClearPending(void)
{
    *vel_control_isr_INTC_CLR_PD = vel_control_isr__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
