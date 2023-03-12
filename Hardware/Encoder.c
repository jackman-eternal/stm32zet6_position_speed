#include "Encoder.h"                // Device header
//电机编码器线数大约位263

void Encoder1_Init()
{
    //开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);      
    //输入比较用的TIM2，所以输入捕获用TIM3，不能用一个，TIM3也在APB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	NVIC_InitTypeDef NVIC_TIM3;
	
	NVIC_TIM3.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_TIM3.NVIC_IRQChannelCmd = ENABLE;
	NVIC_TIM3.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_TIM3.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_TIM3);
    //配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;   //TIM3的通道一的引脚在PA6上
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    //配置时基单元
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR，设大，防止溢出
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
  TIM_ICStructInit(&TIM_ICInitStructure);    //给默认的初始值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;         //通道一
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                //滤波大小 
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;         //通道一
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                //滤波大小  
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
  TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
  //配置编码器接口
		
	TIM_SetCounter(TIM3,0);
  TIM_ClearFlag(TIM3,TIM_FLAG_Update);	
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM3,ENABLE);
}

uint16_t Encoder1_get(void)
{
    int16_t Temp;
    Temp=TIM_GetCounter(TIM3);
	  TIM_SetCounter(TIM3,0);
    return Temp;
}

uint32_t read_encoder(void)
{
	uint32_t num = 0;
	num = TIM3->CNT;  //tim3设置为编码器模式
	return num;
}

int16_t overtime = 0;
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		if(TIM3->CR1 && TIM_CounterMode_Down != TIM_CounterMode_Down)
		{
			//overflow up
			overtime++;
		}
		else
		{
			//overflow down
			overtime--;
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}
//PID周期计算与控制
int pwm_val_protect(int pwm_input)
{
	int pwm_output = 0;
	
	if(pwm_input>99) 
	{
		pwm_output = 99;
	}
	else if(pwm_input<-99) 
	{
		pwm_output = -99;
	}
	else if((pwm_input>-5)&&(pwm_input<5)) 
	{
		pwm_output = 0;
	}
	else
	{
		pwm_output = pwm_input;
	}
	
	return pwm_output;
}

void set_clockwise_rotate(void)
{
	  GPIO_ResetBits(GPIOB, GPIO_Pin_15);//io1
    GPIO_SetBits(GPIOB, GPIO_Pin_14);  //io2
}
void set_anticlockwise_rotate(void)
{
	  GPIO_SetBits(GPIOB, GPIO_Pin_15);
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}
void set_stop_rotate(void)
{
	  GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	  GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}

void set_motor_rotate(int pwm)
{
    if(pwm>10)
	{ 
		if(pwm>99)
		{
			 pwm=99;
		}   
		set_clockwise_rotate();//顺时针
		PWM_SetCompare2(pwm);
		
	}
	else if(pwm<-10)
	{

		if(pwm<-99)
		{
			pwm = 99;
		}
		set_anticlockwise_rotate();
		PWM_SetCompare2(pwm);
	}
	else
	{
		set_stop_rotate();
	}
}

int count =10;
void set_motor_rotate2(int pwm)
{
    if(pwm>0)
	{ 
		//count++;
		//pwm = pwm_control(count);
		  pwm = pwm_control(count+pwm/2);
		set_clockwise_rotate();//顺时针
		PWM_SetCompare2(pwm);
		
	}
	else if(pwm<0)
	{
    //count--;
		pwm = pwm_control(pwm/2+count);
		set_anticlockwise_rotate();
		PWM_SetCompare2(pwm);
	}
	else
	{
		pwm = pwm_control(0);
		set_stop_rotate();
	}
}
