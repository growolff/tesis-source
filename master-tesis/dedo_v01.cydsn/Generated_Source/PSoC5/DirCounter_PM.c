/*******************************************************************************
* File Name: DirCounter_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
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

static DirCounter_BACKUP_STRUCT DirCounter_backup = {0u};


/*******************************************************************************
* Function Name: DirCounter_SaveConfig
********************************************************************************
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DirCounter_SaveConfig(void) 
{
    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT)
        DirCounter_Cnt8_SaveConfig();
    #else 
        /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT) || 
         * (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT)
         */
        DirCounter_Cnt16_SaveConfig();
    #endif /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: DirCounter_RestoreConfig
********************************************************************************
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DirCounter_RestoreConfig(void) 
{
    #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT)
        DirCounter_Cnt8_RestoreConfig();
    #else 
        /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT) || 
         * (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT) 
         */
        DirCounter_Cnt16_RestoreConfig();
    #endif /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: DirCounter_Sleep
********************************************************************************
* 
* Summary:
*  Prepare Quadrature Decoder Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DirCounter_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DirCounter_Sleep(void) 
{
    if (0u != (DirCounter_SR_AUX_CONTROL & DirCounter_INTERRUPTS_ENABLE))
    {
        DirCounter_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        DirCounter_backup.enableState = 0u;
    }

    DirCounter_Stop();
    DirCounter_SaveConfig();
}


/*******************************************************************************
* Function Name: DirCounter_Wakeup
********************************************************************************
*
* Summary:
*  Prepare Quadrature Decoder Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DirCounter_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void DirCounter_Wakeup(void) 
{
    DirCounter_RestoreConfig();

    if (DirCounter_backup.enableState != 0u)
    {
        #if (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT)
            DirCounter_Cnt8_Enable();
        #else 
            /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_16_BIT) || 
            *  (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_32_BIT) 
            */
            DirCounter_Cnt16_Enable();
        #endif /* (DirCounter_COUNTER_SIZE == DirCounter_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        DirCounter_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */

