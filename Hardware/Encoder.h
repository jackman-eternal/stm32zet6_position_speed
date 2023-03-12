#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"  
#include "PID.h"
#include "LED.h"
#include<math.h>
uint16_t Encoder1_get(void);
void Encoder1_Init(void);
uint32_t read_encoder(void);
extern int16_t overtime;
int pwm_val_protect(int pwm_input);
void set_motor_rotate(int pwm);
void set_clockwise_rotate(void);
void set_motor_rotate2(int pwm);

#endif


