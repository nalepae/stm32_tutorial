#include <stm32f0xx.h>

int main(void)
{
	// Enable Peripherals Clocks
	RCC->AHBENR |= ((1UL << 17) | (1UL << 19));
	
	// Configure Pins
	
	// 1. LED
	GPIOC->MODER |= (1UL << 2 * 8);

	// 2. Push button
	// Nothing to do ...
	
	while(1)
	{
		uint32_t butVal;
		
		// Read push button
		butVal = GPIOA->IDR & (1 << 0);
		
		// Toggle blue LED
		GPIOC->BSRR = butVal ? (1 << 8) : (1 << 24);
	}
}
