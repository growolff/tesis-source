/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "structs.h"

/* Functon: FINGER_initFinger
 * -------
 *	set and move the finger to the initial position
 *
 *
 *
 */
void FINGER_initFinger(FINGER_t* finger)
{
	float rvt[3] = {KP_POS, KI_POS, KD_POS}; 
    float spd[3] = {KP_VEL, KI_VEL, KD_VEL}; 
    float tns[3] = {KP_TENS, KI_TENS, KD_TENS};
        
    PIN_t PM1_BR,PM1_DR,PM1_EN;
    PIN_t PM2_BR,PM2_DR,PM2_EN;

    PM1_BR.DR = &PM1_BRAKEn_DR;
    PM1_BR.MASK = PM1_BRAKEn_MASK;
    PM1_BR.STATE = 0;
    PM1_DR.DR = &PM1_DIR_DR;
    PM1_DR.MASK = PM1_DIR_MASK;
    PM1_DR.STATE = 1;
    PM1_EN.DR = &PM1_ENABLE_DR;
    PM1_EN.MASK = PM1_ENABLE_MASK;
    PM1_EN.STATE = 1;
    
    PM2_BR.DR = &PM2_BRAKEn_DR;
    PM2_BR.MASK = PM2_BRAKEn_MASK;
    PM2_BR.STATE = 0;
    PM2_DR.DR = &PM2_DIR_DR;
    PM2_DR.MASK = PM2_DIR_MASK;
    PM2_DR.STATE = 1;
    PM2_EN.DR = &PM2_ENABLE_DR;
    PM2_EN.MASK = PM2_ENABLE_MASK;
    PM2_EN.STATE = 1;
    
    PM1.control_mode = 1;
    PM2.control_mode = 1;

    MOTOR_initControlParams(&PM1,rvt,spd,tns);
    MOTOR_init(&PM1,PM1_EN,PM1_BR,PM1_DR);

}


/* Function: FINGER_setMode
 * --------
 * 	set a movement mode to one finger.
 * 	Control modes are:
 *  1. Open
 *  2. Close
 *  3. Tension Close
 *	4. Tension Reference
 */
void FINGER_setMode(FINGER_t* finger, uint8 command)
{


}


/* [] END OF FILE */
