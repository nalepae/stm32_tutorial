#include <stm32f0xx_conf.h>
#include "uart.h"
#include <string.h>

void Delay(uint32_t nTime);

int main(void)
{
	
	// String to send
	char *stringToSend = "hello world";
	
	uart_open(USART1, 9600, USART_Mode_Rx | USART_Mode_Tx);
	
	// Configure SysTick Timer
	if (SysTick_Config(SystemCoreClock / 1000))
		while(1);
	
	while(1)
	{
		int i;
		for (i = 0; i < strlen(stringToSend); i ++)
			uart_putc(USART1, stringToSend[i]);
		Delay(250); // wait 250ms
	}
}

// Timer code
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void SysTick_Handler(void)
{
	if (TimingDelay != 0x00)
		TimingDelay--;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	/* Infinite loop */
	/* Use GDB to fing out why we're here */
	while(1);
}
#endif
