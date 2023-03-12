#include "motor.h"   // Device header

/*

һ���˿ڽ�PWM������һ���Ӹߵ͵�ƽ�����۽��ĸ���ƽ�����PWM���β��䣬��PWM���30
IO�Ӹ���PWM���ϰ�������ѹ��������30�ĵ�ѹ����ת
IO������PWM���ϰ�������ѹ��������70�ĵ�ѹ����ת

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

