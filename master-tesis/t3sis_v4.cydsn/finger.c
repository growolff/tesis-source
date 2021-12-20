/* ========================================
 *
 * Copyright Gonzalo Olave, 2021
 *
 * This work is published under CC-BY-SA license
 *
 * ========================================
*/

#include "finger.h"

void FINGER_init(FINGER_t* finger, MOTOR_t* mf, MOTOR_t* me){
 
    finger->M[0] = mf;
    finger->M[1] = me;
    
    PID_init(&finger->ten_controller,finger->tension_pid[0],finger->tension_pid[1],finger->tension_pid[2]);
    PID_setMaxValue(&finger->ten_controller, _FINGER_MAX_TENSION);
    PID_setMinValue(&finger->ten_controller, _FINGER_MIN_TENSION);
    
    finger->me_A = 1.5;
    finger->me_L = 36;
    finger->me_R = 0.175;
    
    finger->mf_A = 1.5;
    finger->mf_L = 36;
    finger->mf_R = 0.175;
    
    finger->t0 = 10; // grs
    
}

void FINGER_readTension(FINGER_t* finger)
{
    finger->string_tension = FINGER_getTensionFromReading(finger->pressure_sensor);
}


void FINGER_setTension(FINGER_t* finger)
{
    finger->tensionPID_out = PID_calculatePID(&finger->ten_controller,finger->string_tension);
    
    // relacion entre tension y posicion del motor
    int16_t me_rvt_ref = finger->tensionPID_out;
    
    
    
    switch(finger->idx){
        case 0:
            if(finger->M[F1_ME_IDX]->control_mode == M_FORCE_CONTROL_MODE){
                MOTOR_setRvtRef(finger->M[F1_ME_IDX],me_rvt_ref);
            }
            break;
    }
    
}

void FINGER_setTensionRef(FINGER_t* finger, int16_t ref_tension)
{
    finger->ref_tension = ref_tension;    
    PID_setRef(&finger->ten_controller,ref_tension);
}

void FINGER_setTensionControlParams(FINGER_t* finger, float kp, float ki, float kd)
{
    finger->tension_pid[0] = kp;
    finger->tension_pid[1] = ki;
    finger->tension_pid[2] = kd;

    PID_setCoeffs(&finger->ten_controller,kp,ki,kd);
}

uint16_t FINGER_getTensionFromReading(uint16_t read)
{
    // returns A101 sensor reading in g
    uint16_t tension = 0;
    float c_factor = 0.2908;
    float bit_factor = 4096.0;
    float force = read * 5.0/bit_factor*c_factor; 
    
    // using the ecuation for equilibrum state
    float Df = 0.017; // distance in meters
    float de = 0.0085;
    float alpha = atan(0.003/de) * 3.14159 / 180; // in radians
    float beta = atan(0.003/Df) * 3.14159 / 180; // in radians

    tension = Df*force*cos(alpha) / (de * cos(beta))*1000;

    return tension;
}

void FINGER_open(FINGER_t* finger){
    uint8_t ME_IDX,MF_IDX;
    if(finger->idx == 0){
        ME_IDX = F1_ME_IDX;
        MF_IDX = F1_MF_IDX;
    }
    // move ME to open position
    MOTOR_setRvtRef(finger->M[ME_IDX], F1_ME_OPEN_POS);
    // move MF to open position
    MOTOR_setRvtRef(finger->M[MF_IDX], F1_MF_OPEN_POS);
}

void FINGER_close(FINGER_t* finger){
    uint8_t ME_IDX,MF_IDX;
    if(finger->idx == 0){
        ME_IDX = F1_ME_IDX;
        MF_IDX = F1_MF_IDX;
    }
    // move MF to open position
    MOTOR_setRvtRef(finger->M[MF_IDX], F1_MF_CLOSED_POS);
    // ME motor follows the movement of 
}

void FINGER_setAntagonist(FINGER_t* finger)
{
    
}


void FINGER_setPosition(FINGER_t* finger, int pos)
{
    // control de posicion del dedo
    // motor flector controla la posicion mientras el extensor mantiene la tension
     
    // salida del controlador de posicion son ticks de los sensores del motor, 16 por vuelta
    
    
    /*
    if(motor->rvtPID_out > 0){
        MOTOR_setCW(motor);
    }
    else{
        MOTOR_setCCW(motor);
        motor_PWM = -motor_PWM;
    }
    switch (motor->idx)
    {
        case 0:
            PWM_M1_WriteCompare(motor_PWM);
            break;
        case 1:
            PWM_M2_WriteCompare(motor_PWM);
            break;
    }
    */

}

/* [] END OF FILE */
