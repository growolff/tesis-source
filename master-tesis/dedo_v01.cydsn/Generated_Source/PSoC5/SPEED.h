/*******************************************************************************
* File Name: SPEED.h  
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

#if !defined(CY_PINS_SPEED_H) /* Pins SPEED_H */
#define CY_PINS_SPEED_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SPEED_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SPEED__PORT == 15 && ((SPEED__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    SPEED_Write(uint8 value);
void    SPEED_SetDriveMode(uint8 mode);
uint8   SPEED_ReadDataReg(void);
uint8   SPEED_Read(void);
void    SPEED_SetInterruptMode(uint16 position, uint16 mode);
uint8   SPEED_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the SPEED_SetDriveMode() function.
     *  @{
     */
        #define SPEED_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define SPEED_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define SPEED_DM_RES_UP          PIN_DM_RES_UP
        #define SPEED_DM_RES_DWN         PIN_DM_RES_DWN
        #define SPEED_DM_OD_LO           PIN_DM_OD_LO
        #define SPEED_DM_OD_HI           PIN_DM_OD_HI
        #define SPEED_DM_STRONG          PIN_DM_STRONG
        #define SPEED_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define SPEED_MASK               SPEED__MASK
#define SPEED_SHIFT              SPEED__SHIFT
#define SPEED_WIDTH              1u

/* Interrupt constants */
#if defined(SPEED__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SPEED_SetInterruptMode() function.
     *  @{
     */
        #define SPEED_INTR_NONE      (uint16)(0x0000u)
        #define SPEED_INTR_RISING    (uint16)(0x0001u)
        #define SPEED_INTR_FALLING   (uint16)(0x0002u)
        #define SPEED_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define SPEED_INTR_MASK      (0x01u) 
#endif /* (SPEED__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SPEED_PS                     (* (reg8 *) SPEED__PS)
/* Data Register */
#define SPEED_DR                     (* (reg8 *) SPEED__DR)
/* Port Number */
#define SPEED_PRT_NUM                (* (reg8 *) SPEED__PRT) 
/* Connect to Analog Globals */                                                  
#define SPEED_AG                     (* (reg8 *) SPEED__AG)                       
/* Analog MUX bux enable */
#define SPEED_AMUX                   (* (reg8 *) SPEED__AMUX) 
/* Bidirectional Enable */                                                        
#define SPEED_BIE                    (* (reg8 *) SPEED__BIE)
/* Bit-mask for Aliased Register Access */
#define SPEED_BIT_MASK               (* (reg8 *) SPEED__BIT_MASK)
/* Bypass Enable */
#define SPEED_BYP                    (* (reg8 *) SPEED__BYP)
/* Port wide control signals */                                                   
#define SPEED_CTL                    (* (reg8 *) SPEED__CTL)
/* Drive Modes */
#define SPEED_DM0                    (* (reg8 *) SPEED__DM0) 
#define SPEED_DM1                    (* (reg8 *) SPEED__DM1)
#define SPEED_DM2                    (* (reg8 *) SPEED__DM2) 
/* Input Buffer Disable Override */
#define SPEED_INP_DIS                (* (reg8 *) SPEED__INP_DIS)
/* LCD Common or Segment Drive */
#define SPEED_LCD_COM_SEG            (* (reg8 *) SPEED__LCD_COM_SEG)
/* Enable Segment LCD */
#define SPEED_LCD_EN                 (* (reg8 *) SPEED__LCD_EN)
/* Slew Rate Control */
#define SPEED_SLW                    (* (reg8 *) SPEED__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SPEED_PRTDSI__CAPS_SEL       (* (reg8 *) SPEED__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SPEED_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SPEED__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SPEED_PRTDSI__OE_SEL0        (* (reg8 *) SPEED__PRTDSI__OE_SEL0) 
#define SPEED_PRTDSI__OE_SEL1        (* (reg8 *) SPEED__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SPEED_PRTDSI__OUT_SEL0       (* (reg8 *) SPEED__PRTDSI__OUT_SEL0) 
#define SPEED_PRTDSI__OUT_SEL1       (* (reg8 *) SPEED__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SPEED_PRTDSI__SYNC_OUT       (* (reg8 *) SPEED__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(SPEED__SIO_CFG)
    #define SPEED_SIO_HYST_EN        (* (reg8 *) SPEED__SIO_HYST_EN)
    #define SPEED_SIO_REG_HIFREQ     (* (reg8 *) SPEED__SIO_REG_HIFREQ)
    #define SPEED_SIO_CFG            (* (reg8 *) SPEED__SIO_CFG)
    #define SPEED_SIO_DIFF           (* (reg8 *) SPEED__SIO_DIFF)
#endif /* (SPEED__SIO_CFG) */

/* Interrupt Registers */
#if defined(SPEED__INTSTAT)
    #define SPEED_INTSTAT            (* (reg8 *) SPEED__INTSTAT)
    #define SPEED_SNAP               (* (reg8 *) SPEED__SNAP)
    
	#define SPEED_0_INTTYPE_REG 		(* (reg8 *) SPEED__0__INTTYPE)
#endif /* (SPEED__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SPEED_H */


/* [] END OF FILE */
