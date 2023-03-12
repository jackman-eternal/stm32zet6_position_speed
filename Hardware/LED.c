#include "stm32f10x.h"                  // Device header
void LED_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    //APB2ʹ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12|GPIO_Pin_15|GPIO_Pin_14;     //��ʼ��PA2��PA3����������ߵ͵�ƽ               
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

