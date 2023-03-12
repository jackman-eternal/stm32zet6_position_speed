#include "serial.h"                 // Device header
#include <stdio.h>                  //用printf函数打印
#include <stdarg.h>                  //用来封装sprintf所需函数

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  //开启ustar的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //开启引脚时钟
	
    //GPIOA初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//输入流控，NONE为不用流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx;//串口模式，TX发送模式
	USART_InitStructure.USART_Parity = USART_Parity_No;//无效应
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位，1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长，无效应一般选择8位
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);//使能
}

void Serial_SendByte(uint8_t Byte)    //发送一个字节
{
	USART_SendData(USART1, Byte);    //发送数据，BYTE写入TDR
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待下一个标志位，标志位置1后会自动清零，
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)     //发送一个数组，数组和长度
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);     //一次发送数组中的元素
	}
}

void Serial_SendString(char *String)//发送字符串
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);//一个个发送
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)    //拆分数字用的子函数
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)         //发送数字，数字的位数
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');   //拆分数字的每一个数发送
	}
}

int fputc(int ch, FILE *f)          //重定向fputc到串口，fputc是printf的底层，fputc一个个打印到printf
{
	Serial_SendByte(ch);     //一个个发送
	return ch;
}

void Serial_Printf(char *format, ...)     //封装sprintf
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}
