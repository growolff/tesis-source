/*******************************************************************************
* File Name: ENABLE.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_ENABLE_H) /* Pins ENABLE_H */
#define CY_PINS_ENABLE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ENABLE_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ENABLE__PORT == 15 && ((ENABLE__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ENABLE_Write(uint8 value);
void    ENABLE_SetDriveMode(uint8 mode);
uint8   ENABLE_ReadDataReg(void);
uint8   ENABLE_Read(void);
void    ENABLE_SetInterruptMode(uint16 position, uint16 mode);
uint8   ENABLE_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ENABLE_SetDriveMode() function.
     *  @{
     */
        #define ENABLE_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ENABLE_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ENABLE_DM_RES_UP          PIN_DM_RES_UP
        #define ENABLE_DM_RES_DWN         PIN_DM_RES_DWN
        #define ENABLE_DM_OD_LO           PIN_DM_OD_LO
        #define ENABLE_DM_OD_HI           PIN_DM_OD_HI
        #define ENABLE_DM_STRONG          PIN_DM_STRONG
        #define ENABLE_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ENABLE_MASK               ENABLE__MASK
#define ENABLE_SHIFT              ENABLE__SHIFT
#define ENABLE_WIDTH              1u

/* Interrupt constants */
#if defined(ENABLE__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ENABLE_SetInterruptMode() function.
     *  @{
     */
        #define ENABLE_INTR_NONE      (uint16)(0x0000u)
        #define ENABLE_INTR_RISING    (uint16)(0x0001u)
        #define ENABLE_INTR_FALLING   (uint16)(0x0002u)
        #define ENABLE_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ENABLE_INTR_MASK      (0x01u) 
#endif /* (ENABLE__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ENABLE_PS                     (* (reg8 *) ENABLE__PS)
/* Data Register */
#define ENABLE_DR                     (* (reg8 *) ENABLE__DR)
/* Port Number */
#define ENABLE_PRT_NUM                (* (reg8 *) ENABLE__PRT) 
/* Connect to Analog Globals */                                                  
#define ENABLE_AG                     (* (reg8 *) ENABLE__AG)                       
/* Analog MUX bux enable */
#define ENABLE_AMUX                   (* (reg8 *) ENABLE__AMUX) 
/* Bidirectional Enable */                                                        
#define ENABLE_BIE                    (* (reg8 *) ENABLE__BIE)
/* Bit-mask for Aliased Register Access */
#define ENABLE_BIT_MASK               (* (reg8 *) ENABLE__BIT_MASK)
/* Bypass Enable */
#define ENABLE_BYP                    (* (reg8 *) ENABLE__BYP)
/* Port wide control signals */                                                   
#define ENABLE_CTL                    (* (reg8 *) ENABLE__CTL)
/* Drive Modes */
#define ENABLE_DM0                    (* (reg8 *) ENABLE__DM0) 
#define ENABLE_DM1                    (* (reg8 *) ENABLE__DM1)
#define ENABLE_DM2                    (* (reg8 *) ENABLE__DM2) 
/* Input Buffer Disable Override */
#define ENABLE_INP_DIS                (* (reg8 *) ENABLE__INP_DIS)
/* LCD Common or Segment Drive */
#define ENABLE_LCD_COM_SEG            (* (reg8 *) ENABLE__LCD_COM_SEG)
/* Enable Segment LCD */
#define ENABLE_LCD_EN                 (* (reg8 *) ENABLE__LCD_EN)
/* Slew Rate Control */
#define ENABLE_SLW                    (* (reg8 *) ENABLE__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ENABLE_PRTDSI__CAPS_SEL       (* (reg8 *) ENABLE__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ENABLE_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ENABLE__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ENABLE_PRTDSI__OE_SEL0        (* (reg8 *) ENABLE__PRTDSI__OE_SEL0) 
#define ENABLE_PRTDSI__OE_SEL1        (* (reg8 *) ENABLE__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ENABLE_PRTDSI__OUT_SEL0       (* (reg8 *) ENABLE__PRTDSI__OUT_SEL0) 
#define ENABLE_PRTDSI__OUT_SEL1       (* (reg8 *) ENABLE__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ENABLE_PRTDSI__SYNC_OUT       (* (reg8 *) ENABLE__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ENABLE__SIO_CFG)
    #define ENABLE_SIO_HYST_EN        (* (reg8 *) ENABLE__SIO_HYST_EN)
    #define ENABLE_SIO_REG_HIFREQ     (* (reg8 *) ENABLE__SIO_REG_HIFREQ)
    #define ENABLE_SIO_CFG            (* (reg8 *) ENABLE__SIO_CFG)
    #define ENABLE_SIO_DIFF           (* (reg8 *) ENABLE__SIO_DIFF)
#endif /* (ENABLE__SIO_CFG) */

/* Interrupt Registers */
#if defined(ENABLE__INTSTAT)
    #define ENABLE_INTSTAT            (* (reg8 *) ENABLE__INTSTAT)
    #define ENABLE_SNAP               (* (reg8 *) ENABLE__SNAP)
    
	#define ENABLE_0_INTTYPE_REG 		(* (reg8 *) ENABLE__0__INTTYPE)
#endif /* (ENABLE__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ENABLE_H */


/* [] END OF FILE */
