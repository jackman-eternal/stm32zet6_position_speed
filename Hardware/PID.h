#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"
#include "pwm.h"
#include "PID.h"
#include "Encoder.h"
#include "Serial.h"
#include "motor.h" 
float PID_realize(float actual_val,float tar1);
void autocallback(void);
float pid1(int16_t speed1,float tar1);

int pwm_control(int pwm);
int16_t myabs(int a);
int speed_val_protect(int val);
float speed_pid_realize(float actual_val,float tar1);

extern float Err;
extern float Err2;
#endif


