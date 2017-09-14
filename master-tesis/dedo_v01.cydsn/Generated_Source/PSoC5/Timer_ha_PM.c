/*******************************************************************************
* File Name: Timer_ha_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Timer_ha.h"

static Timer_ha_backupStruct Timer_ha_backup;


/*******************************************************************************
* Function Name: Timer_ha_SaveConfig
********************************************************************************
*
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
*  Timer_ha_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_ha_SaveConfig(void) 
{
    #if (!Timer_ha_UsingFixedFunction)
        Timer_ha_backup.TimerUdb = Timer_ha_ReadCounter();
        Timer_ha_backup.InterruptMaskValue = Timer_ha_STATUS_MASK;
        #if (Timer_ha_UsingHWCaptureCounter)
            Timer_ha_backup.TimerCaptureCounter = Timer_ha_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_ha_UDB_CONTROL_REG_REMOVED)
            Timer_ha_backup.TimerControlRegister = Timer_ha_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_ha_RestoreConfig
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
*  Timer_ha_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_ha_RestoreConfig(void) 
{   
    #if (!Timer_ha_UsingFixedFunction)

        Timer_ha_WriteCounter(Timer_ha_backup.TimerUdb);
        Timer_ha_STATUS_MASK =Timer_ha_backup.InterruptMaskValue;
        #if (Timer_ha_UsingHWCaptureCounter)
            Timer_ha_SetCaptureCount(Timer_ha_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_ha_UDB_CONTROL_REG_REMOVED)
            Timer_ha_WriteControlRegister(Timer_ha_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_ha_Sleep
********************************************************************************
*
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
*  Timer_ha_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_ha_Sleep(void) 
{
    #if(!Timer_ha_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_ha_CTRL_ENABLE == (Timer_ha_CONTROL & Timer_ha_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_ha_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_ha_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_ha_Stop();
    Timer_ha_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_ha_Wakeup
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
*  Timer_ha_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_ha_Wakeup(void) 
{
    Timer_ha_RestoreConfig();
    #if(!Timer_ha_UDB_CONTROL_REG_REMOVED)
        if(Timer_ha_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_ha_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
