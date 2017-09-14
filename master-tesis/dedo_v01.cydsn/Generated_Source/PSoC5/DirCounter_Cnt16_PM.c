/*******************************************************************************
* File Name: DirCounter_Cnt16_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "DirCounter_Cnt16.h"

static DirCounter_Cnt16_backupStruct DirCounter_Cnt16_backup;


/*******************************************************************************
* Function Name: DirCounter_Cnt16_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  DirCounter_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void DirCounter_Cnt16_SaveConfig(void) 
{
    #if (!DirCounter_Cnt16_UsingFixedFunction)

        DirCounter_Cnt16_backup.CounterUdb = DirCounter_Cnt16_ReadCounter();

        #if(!DirCounter_Cnt16_ControlRegRemoved)
            DirCounter_Cnt16_backup.CounterControlRegister = DirCounter_Cnt16_ReadControlRegister();
        #endif /* (!DirCounter_Cnt16_ControlRegRemoved) */

    #endif /* (!DirCounter_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: DirCounter_Cnt16_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  DirCounter_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void DirCounter_Cnt16_RestoreConfig(void) 
{      
    #if (!DirCounter_Cnt16_UsingFixedFunction)

       DirCounter_Cnt16_WriteCounter(DirCounter_Cnt16_backup.CounterUdb);

        #if(!DirCounter_Cnt16_ControlRegRemoved)
            DirCounter_Cnt16_WriteControlRegister(DirCounter_Cnt16_backup.CounterControlRegister);
        #endif /* (!DirCounter_Cnt16_ControlRegRemoved) */

    #endif /* (!DirCounter_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: DirCounter_Cnt16_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  DirCounter_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void DirCounter_Cnt16_Sleep(void) 
{
    #if(!DirCounter_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(DirCounter_Cnt16_CTRL_ENABLE == (DirCounter_Cnt16_CONTROL & DirCounter_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            DirCounter_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            DirCounter_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        DirCounter_Cnt16_backup.CounterEnableState = 1u;
        if(DirCounter_Cnt16_backup.CounterEnableState != 0u)
        {
            DirCounter_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!DirCounter_Cnt16_ControlRegRemoved) */
    
    DirCounter_Cnt16_Stop();
    DirCounter_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: DirCounter_Cnt16_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  DirCounter_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void DirCounter_Cnt16_Wakeup(void) 
{
    DirCounter_Cnt16_RestoreConfig();
    #if(!DirCounter_Cnt16_ControlRegRemoved)
        if(DirCounter_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            DirCounter_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!DirCounter_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */
