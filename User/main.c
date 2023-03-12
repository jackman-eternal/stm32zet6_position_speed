#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "serial.h"
#include "pwm.h"
#include "motor.h"
#include "stdio.h"
#include  "ENCODER.H"
#include "PID.H"
#include "led.h"

int16_t speed,time=0;

int main(void)
{
	  OLED_Init();
    LED_Init();
    Serial_Init();
    PWM_Init();
    Encoder1_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    PWM_SetCompare2(0);
//  OLED_ShowString(1, 1, "speed:");
	while (1)
	{
    // right_f(pid1(speed,tar));
		   Delay_ms(1000);
	//	 printf("speed:%d\r\n",speed);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
        time++;
        if(time==20)
        {
					  autocallback();
           // speed=Encoder1_get();
            time=0;
        }
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}





