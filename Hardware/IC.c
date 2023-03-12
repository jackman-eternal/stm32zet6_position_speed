#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
    //开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);      
    //输入比较用的TIM2，所以输入捕获用TIM3，不能用一个，TIM3也在APB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    //配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   //TIM3的通道一的引脚在PA6上
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    //配置时基单元
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR，设大，防止溢出
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;         //通道一
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                //滤波大小
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;   //设置级性，上升沿触发
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     //设置分频，1分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   //选择触发信号从那个引脚输入，直连通道
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);            
    //TIM_PWMIConfig自动将通道2配置成相反的模式，交叉捕获，通道1捕获上升沿，通道2捕获下降沿
    //此函数只支持通道1和通道2的配置
	
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);      //触发源选择，TI1FP1
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);   //选择从模式
		
	TIM_Cmd(TIM3, ENABLE);           //启动定时器
}

//测周法
uint16_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3)+1);
    /*
    f=1/T
    T=n/fc
    fx=1/Tx=fc/n
    fc=72M/(psc+1)      //fc是标准频率
    n就是读取CCR的值
    */
}
uint16_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
    //CCR2/CCR1即为占空比
}
//高电平的计数值存在CCR2，整个周期的计数值存在CCR1里
