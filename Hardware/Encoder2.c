#include "stm32f10x.h"                  // Device header
//电机编码器线数大约位263
void Encoder2_Init()
{
    //开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);      
    //输入比较用的TIM2，所以输入捕获用TIM3，不能用一个，TIM3也在APB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
    //配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;   //TIM3的通道一的引脚在PA6上
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    //配置时基单元
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR，设大，防止溢出
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);    //给默认的初始值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;         //通道一
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                //滤波大小 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;         //通道一
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                //滤波大小  
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
    
    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    //配置编码器接口
    
    TIM_Cmd(TIM4,ENABLE);
}

int16_t Encoder2_get(void)
{
    int16_t Temp;
    Temp=TIM_GetCounter(TIM4);
    TIM_SetCounter(TIM4,0);
    return Temp;
}
