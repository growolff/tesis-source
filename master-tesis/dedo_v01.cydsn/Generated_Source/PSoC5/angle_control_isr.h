/*******************************************************************************
* File Name: angle_control_isr.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_angle_control_isr_H)
#define CY_ISR_angle_control_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void angle_control_isr_Start(void);
void angle_control_isr_StartEx(cyisraddress address);
void angle_control_isr_Stop(void);

CY_ISR_PROTO(angle_control_isr_Interrupt);

void angle_control_isr_SetVector(cyisraddress address);
cyisraddress angle_control_isr_GetVector(void);

void angle_control_isr_SetPriority(uint8 priority);
uint8 angle_control_isr_GetPriority(void);

void angle_control_isr_Enable(void);
uint8 angle_control_isr_GetState(void);
void angle_control_isr_Disable(void);

void angle_control_isr_SetPending(void);
void angle_control_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the angle_control_isr ISR. */
#define angle_control_isr_INTC_VECTOR            ((reg32 *) angle_control_isr__INTC_VECT)

/* Address of the angle_control_isr ISR priority. */
#define angle_control_isr_INTC_PRIOR             ((reg8 *) angle_control_isr__INTC_PRIOR_REG)

/* Priority of the angle_control_isr interrupt. */
#define angle_control_isr_INTC_PRIOR_NUMBER      angle_control_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable angle_control_isr interrupt. */
#define angle_control_isr_INTC_SET_EN            ((reg32 *) angle_control_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the angle_control_isr interrupt. */
#define angle_control_isr_INTC_CLR_EN            ((reg32 *) angle_control_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the angle_control_isr interrupt state to pending. */
#define angle_control_isr_INTC_SET_PD            ((reg32 *) angle_control_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the angle_control_isr interrupt. */
#define angle_control_isr_INTC_CLR_PD            ((reg32 *) angle_control_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_angle_control_isr_H */


/* [] END OF FILE */
