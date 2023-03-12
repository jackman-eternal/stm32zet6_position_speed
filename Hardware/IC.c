#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
    //����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);      
    //����Ƚ��õ�TIM2���������벶����TIM3��������һ����TIM3Ҳ��APB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    //����GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   //TIM3��ͨ��һ��������PA6��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    //����ʱ����Ԫ
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR����󣬷�ֹ���
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;         //ͨ��һ
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                //�˲���С
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;   //���ü��ԣ������ش���
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     //���÷�Ƶ��1��Ƶ
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   //ѡ�񴥷��źŴ��Ǹ��������룬ֱ��ͨ��
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);            
    //TIM_PWMIConfig�Զ���ͨ��2���ó��෴��ģʽ�����沶��ͨ��1���������أ�ͨ��2�����½���
    //�˺���ֻ֧��ͨ��1��ͨ��2������
	
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);      //����Դѡ��TI1FP1
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);   //ѡ���ģʽ
		
	TIM_Cmd(TIM3, ENABLE);           //������ʱ��
}

//���ܷ�
uint16_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3)+1);
    /*
    f=1/T
    T=n/fc
    fx=1/Tx=fc/n
    fc=72M/(psc+1)      //fc�Ǳ�׼Ƶ��
    n���Ƕ�ȡCCR��ֵ
    */
}
uint16_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
    //CCR2/CCR1��Ϊռ�ձ�
}
//�ߵ�ƽ�ļ���ֵ����CCR2���������ڵļ���ֵ����CCR1��
