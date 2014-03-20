#include "uart.h"

int uart_open(USART_TypeDef* USARTx, uint32_t baud, uint32_t flags)
{
	// This function assumes GPIOA is used for USART

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	GPIO_StructInit(&GPIO_InitStructure);
	
	// Enable GPIOA Clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	if(USARTx == USART1)
	{
		// Enable USART1 Clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		
		// Initalise Pin Alternate Function Config
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
		
		// Initialise USART1_Tx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		// Initialise USART1_Rx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if(USARTx == USART2)
	{
		// Enable USART2 Clock
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		// Initalise Pin Alternate Function Config
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
		
		// Initialise USART2_Tx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		// Initialise USART2_Rx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}

	// Initialise USART structure
	USART_StructInit(&USART_InitStructure);
	
	// Modify USART_InitStructure for non-default values, e.G.
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_Mode = flags;
	USART_Init(USARTx, &USART_InitStructure);
	USART_Cmd(USARTx, ENABLE);
	
	return 0;
}

char uart_getc(USART_TypeDef* USARTx)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USARTx);
}
int uart_putc(USART_TypeDef* USARTx, char c)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, c);
	
	return 0;
}
