/*******************************************************************************
* File Name: BRAKEn.h  
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

#if !defined(CY_PINS_BRAKEn_H) /* Pins BRAKEn_H */
#define CY_PINS_BRAKEn_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BRAKEn_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BRAKEn__PORT == 15 && ((BRAKEn__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BRAKEn_Write(uint8 value);
void    BRAKEn_SetDriveMode(uint8 mode);
uint8   BRAKEn_ReadDataReg(void);
uint8   BRAKEn_Read(void);
void    BRAKEn_SetInterruptMode(uint16 position, uint16 mode);
uint8   BRAKEn_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BRAKEn_SetDriveMode() function.
     *  @{
     */
        #define BRAKEn_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BRAKEn_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BRAKEn_DM_RES_UP          PIN_DM_RES_UP
        #define BRAKEn_DM_RES_DWN         PIN_DM_RES_DWN
        #define BRAKEn_DM_OD_LO           PIN_DM_OD_LO
        #define BRAKEn_DM_OD_HI           PIN_DM_OD_HI
        #define BRAKEn_DM_STRONG          PIN_DM_STRONG
        #define BRAKEn_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BRAKEn_MASK               BRAKEn__MASK
#define BRAKEn_SHIFT              BRAKEn__SHIFT
#define BRAKEn_WIDTH              1u

/* Interrupt constants */
#if defined(BRAKEn__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BRAKEn_SetInterruptMode() function.
     *  @{
     */
        #define BRAKEn_INTR_NONE      (uint16)(0x0000u)
        #define BRAKEn_INTR_RISING    (uint16)(0x0001u)
        #define BRAKEn_INTR_FALLING   (uint16)(0x0002u)
        #define BRAKEn_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BRAKEn_INTR_MASK      (0x01u) 
#endif /* (BRAKEn__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BRAKEn_PS                     (* (reg8 *) BRAKEn__PS)
/* Data Register */
#define BRAKEn_DR                     (* (reg8 *) BRAKEn__DR)
/* Port Number */
#define BRAKEn_PRT_NUM                (* (reg8 *) BRAKEn__PRT) 
/* Connect to Analog Globals */                                                  
#define BRAKEn_AG                     (* (reg8 *) BRAKEn__AG)                       
/* Analog MUX bux enable */
#define BRAKEn_AMUX                   (* (reg8 *) BRAKEn__AMUX) 
/* Bidirectional Enable */                                                        
#define BRAKEn_BIE                    (* (reg8 *) BRAKEn__BIE)
/* Bit-mask for Aliased Register Access */
#define BRAKEn_BIT_MASK               (* (reg8 *) BRAKEn__BIT_MASK)
/* Bypass Enable */
#define BRAKEn_BYP                    (* (reg8 *) BRAKEn__BYP)
/* Port wide control signals */                                                   
#define BRAKEn_CTL                    (* (reg8 *) BRAKEn__CTL)
/* Drive Modes */
#define BRAKEn_DM0                    (* (reg8 *) BRAKEn__DM0) 
#define BRAKEn_DM1                    (* (reg8 *) BRAKEn__DM1)
#define BRAKEn_DM2                    (* (reg8 *) BRAKEn__DM2) 
/* Input Buffer Disable Override */
#define BRAKEn_INP_DIS                (* (reg8 *) BRAKEn__INP_DIS)
/* LCD Common or Segment Drive */
#define BRAKEn_LCD_COM_SEG            (* (reg8 *) BRAKEn__LCD_COM_SEG)
/* Enable Segment LCD */
#define BRAKEn_LCD_EN                 (* (reg8 *) BRAKEn__LCD_EN)
/* Slew Rate Control */
#define BRAKEn_SLW                    (* (reg8 *) BRAKEn__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BRAKEn_PRTDSI__CAPS_SEL       (* (reg8 *) BRAKEn__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BRAKEn_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BRAKEn__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BRAKEn_PRTDSI__OE_SEL0        (* (reg8 *) BRAKEn__PRTDSI__OE_SEL0) 
#define BRAKEn_PRTDSI__OE_SEL1        (* (reg8 *) BRAKEn__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BRAKEn_PRTDSI__OUT_SEL0       (* (reg8 *) BRAKEn__PRTDSI__OUT_SEL0) 
#define BRAKEn_PRTDSI__OUT_SEL1       (* (reg8 *) BRAKEn__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BRAKEn_PRTDSI__SYNC_OUT       (* (reg8 *) BRAKEn__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BRAKEn__SIO_CFG)
    #define BRAKEn_SIO_HYST_EN        (* (reg8 *) BRAKEn__SIO_HYST_EN)
    #define BRAKEn_SIO_REG_HIFREQ     (* (reg8 *) BRAKEn__SIO_REG_HIFREQ)
    #define BRAKEn_SIO_CFG            (* (reg8 *) BRAKEn__SIO_CFG)
    #define BRAKEn_SIO_DIFF           (* (reg8 *) BRAKEn__SIO_DIFF)
#endif /* (BRAKEn__SIO_CFG) */

/* Interrupt Registers */
#if defined(BRAKEn__INTSTAT)
    #define BRAKEn_INTSTAT            (* (reg8 *) BRAKEn__INTSTAT)
    #define BRAKEn_SNAP               (* (reg8 *) BRAKEn__SNAP)
    
	#define BRAKEn_0_INTTYPE_REG 		(* (reg8 *) BRAKEn__0__INTTYPE)
#endif /* (BRAKEn__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BRAKEn_H */


/* [] END OF FILE */
