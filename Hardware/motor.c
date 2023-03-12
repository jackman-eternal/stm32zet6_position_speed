#include "motor.h"   // Device header

/*

一个端口接PWM，还有一个接高低电平，无论接哪个电平输出的PWM波形不变，若PWM输出30
IO接负：PWM的上半段输出电压，则给电机30的电压，正转
IO接正；PWM的上半段输出电压，则给电机70的电压，负转

*/
void left_f(uint8_t speed)       //A0+B0+B12
{
    PWM_SetCompare1(speed);       
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
void left_b(uint8_t speed)       //A0+B0+B12
{
    PWM_SetCompare1(speed);       
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}
void right_f(uint8_t speed)       //A1+B13+B14
{
    PWM_SetCompare2(speed);       
    GPIO_ResetBits(GPIOB, GPIO_Pin_15);
    GPIO_SetBits(GPIOB, GPIO_Pin_14);
}
void right_b(uint8_t speed)       //A1+B13+B14
{
    PWM_SetCompare2(speed);       
    GPIO_SetBits(GPIOB, GPIO_Pin_15);
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

