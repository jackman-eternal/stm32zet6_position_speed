#include "stm32f10x.h"                  // Device header
#include "motor.h"
#include "pid.h"
 
 void My_USART3_Init(void)  
{  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIO????  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//??????  
    
    GPIO_InitTypeDef GPIO_InitStrue;  
    USART_InitTypeDef USART_InitStrue;  
    NVIC_InitTypeDef NVIC_InitStrue;  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOB,&GPIO_InitStrue);  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_11;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOB,&GPIO_InitStrue);  
      
    USART_InitStrue.USART_BaudRate=9600;  
    USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  
    USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;  
    USART_InitStrue.USART_Parity=USART_Parity_No;  
    USART_InitStrue.USART_StopBits=USART_StopBits_1;  
    USART_InitStrue.USART_WordLength=USART_WordLength_8b;  
      
    USART_Init(USART3,&USART_InitStrue);
      
    USART_Cmd(USART3,ENABLE);//????1  
      
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//??????  
      
    NVIC_InitStrue.NVIC_IRQChannel=USART3_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
      
}  
  
void control(u8 res,int16_t speed1,int16_t speed2,int16_t tar1,int16_t tar2)
{
        if(res==49)              //Ç°½ø
        {
            left_f(pid1(speed1,tar1));
            right_f(pid2(speed2,tar2));
        }
        else if(res==48)            //Í£Ö¹
        {
            left_f(pid1(speed1,0));
            right_f(pid2(speed2,0));
            left_f(0);
            right_f(0);
        }
        else if(res==50)                //ºóÍË
        {
            left_b(pid1(speed1,tar1));
            right_b(pid2(speed2,tar2));
        }
}

   
