/*******************************************************************************
* File Name: DirCounter.h  
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the Quadrature
*  Decoder component.
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

#if !defined(CY_QUADRATURE_DECODER_DirCounter_H)
#define CY_QUADRATURE_DECODER_DirCounter_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define DirCounter_COUNTER_SIZE               (32u)
#define DirCounter_COUNTER_SIZE_8_BIT         (8u)
#define DirCounter_COUNTER_SIZE_16_BIT        (16u)
#define DirCounter_COUNTER_SIZE_32_BIT        (32u)

#if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT)
    #include "DirCounter_Cnt8.h"
#else 
    /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT) || 
    *  (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT) 
    */
    #include "DirCounter_Cnt16.h"
#endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT */

extern uint8 DirCounter_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define DirCounter_COUNTER_RESOLUTION         (1u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} DirCounter_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  DirCounter_Init(void) ;
void  DirCounter_Start(void) ;
void  DirCounter_Stop(void) ;
void  DirCounter_Enable(void) ;
uint8 DirCounter_GetEvents(void) ;
void  DirCounter_SetInterruptMask(uint8 mask) ;
uint8 DirCounter_GetInterruptMask(void) ;
int32 DirCounter_GetCounter(void) ;
void  DirCounter_SetCounter(int32 value)
;
void  DirCounter_Sleep(void) ;
void  DirCounter_Wakeup(void) ;
void  DirCounter_SaveConfig(void) ;
void  DirCounter_RestoreConfig(void) ;

#if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(DirCounter_ISR);
#endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
    #define DirCounter_ISR_NUMBER             ((uint8) DirCounter_isr__INTC_NUMBER)
    #define DirCounter_ISR_PRIORITY           ((uint8) DirCounter_isr__INTC_PRIOR_NUM)
#endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define DirCounter_GLITCH_FILTERING           (1u)
#define DirCounter_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT)
    #define DirCounter_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT) ||
    *  (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
    */
    #define DirCounter_COUNTER_INIT_VALUE    (0x8000u)
    #define DirCounter_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define DirCounter_STATUS_REG                 (* (reg8 *) DirCounter_bQuadDec_Stsreg__STATUS_REG)
#define DirCounter_STATUS_PTR                 (  (reg8 *) DirCounter_bQuadDec_Stsreg__STATUS_REG)
#define DirCounter_STATUS_MASK                (* (reg8 *) DirCounter_bQuadDec_Stsreg__MASK_REG)
#define DirCounter_STATUS_MASK_PTR            (  (reg8 *) DirCounter_bQuadDec_Stsreg__MASK_REG)
#define DirCounter_SR_AUX_CONTROL             (* (reg8 *) DirCounter_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define DirCounter_SR_AUX_CONTROL_PTR         (  (reg8 *) DirCounter_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define DirCounter_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define DirCounter_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define DirCounter_COUNTER_RESET_SHIFT        (0x02u)
#define DirCounter_INVALID_IN_SHIFT           (0x03u)
#define DirCounter_COUNTER_OVERFLOW           ((uint8) (0x01u << DirCounter_COUNTER_OVERFLOW_SHIFT))
#define DirCounter_COUNTER_UNDERFLOW          ((uint8) (0x01u << DirCounter_COUNTER_UNDERFLOW_SHIFT))
#define DirCounter_COUNTER_RESET              ((uint8) (0x01u << DirCounter_COUNTER_RESET_SHIFT))
#define DirCounter_INVALID_IN                 ((uint8) (0x01u << DirCounter_INVALID_IN_SHIFT))

#define DirCounter_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define DirCounter_INTERRUPTS_ENABLE          ((uint8)(0x01u << DirCounter_INTERRUPTS_ENABLE_SHIFT))
#define DirCounter_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define DirCounter_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_DirCounter_H */


/* [] END OF FILE */
