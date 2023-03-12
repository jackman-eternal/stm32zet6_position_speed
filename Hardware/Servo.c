#include "stm32f10x.h"                  // Device header
#include "PWM1.h"

void Servo_Init(void)
{
	PWM1_Init();
}

void Servo_SetAngle(float Angle)
{
	PWM1_SetCompare1(Angle / 180 * 2000 + 500);
}

