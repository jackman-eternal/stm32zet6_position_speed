#include "serial.h"                 // Device header
#include <stdio.h>                  //��printf������ӡ
#include <stdarg.h>                  //������װsprintf���躯��

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  //����ustar��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //��������ʱ��
	
    //GPIOA��ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//�������أ�NONEΪ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx;//����ģʽ��TX����ģʽ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��ЧӦ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�����ЧӦһ��ѡ��8λ
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);//ʹ��
}

void Serial_SendByte(uint8_t Byte)    //����һ���ֽ�
{
	USART_SendData(USART1, Byte);    //�������ݣ�BYTEд��TDR
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//�ȴ���һ����־λ����־λ��1����Զ����㣬
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)     //����һ�����飬����ͳ���
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);     //һ�η��������е�Ԫ��
	}
}

void Serial_SendString(char *String)//�����ַ���
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);//һ��������
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)    //��������õ��Ӻ���
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)         //�������֣����ֵ�λ��
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');   //������ֵ�ÿһ��������
	}
}

int fputc(int ch, FILE *f)          //�ض���fputc�����ڣ�fputc��printf�ĵײ㣬fputcһ������ӡ��printf
{
	Serial_SendByte(ch);     //һ��������
	return ch;
}

void Serial_Printf(char *format, ...)     //��װsprintf
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}
