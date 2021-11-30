/* ========================================
 *
 * Copyright Gonzalo Olave, 2021
 *
 * This work is published under CC-BY-SA license
 *
 * ========================================
*/

#ifndef FINGER_H_
#define FINGER_H_
    
#include "pid_o.h"
#include "project.h"
#include "math.h"
#include "motor.h"   
#include <stdio.h>
    
// finger tension limits in grams
#define _FINGER_MIN_TENSION 0
#define _FINGER_MAX_TENSION 1500
    
#define F1_MF M1
#define F1_ME M2    
#define F1_MF_IDX 0
#define F1_ME_IDX 1
    
typedef struct FINGER_t {
    
    MOTOR_t* M[2];
    
    struct PID_t ten_controller; // finger tension controller
    
    float tension_pid[3];
    
    uint8_t idx;
    
    uint16_t pressure_sensor;
    uint16_t string_tension;
    
    uint16_t t0;
    
    int16_t tensionPID_out;
    uint16_t ref_tension;
    
    
} FINGER_t;

void FINGER_setTension(FINGER_t* finger);
void FINGER_setTensionRef(FINGER_t* finger, int16_t ref_tension);
void FINGER_readTension(FINGER_t* finger);

void FINGER_setTensionControlParams(FINGER_t* finger, float kp, float ki, float kd);
uint16_t FINGER_getTensionFromReading(uint16_t read);

// finger, flector motor, extensor motor
void FINGER_init(FINGER_t* finger, MOTOR_t *mf, MOTOR_t *me);

void FINGER_setPosition(FINGER_t* finger);

#endif
/* [] END OF FILE */
