#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h" 
#include "pwm.h"
#include "PID.h" 
void left_f(uint8_t speed);
void left_b(uint8_t speed);
void right_f(uint8_t speed);
void right_b(uint8_t speed);

#endif
