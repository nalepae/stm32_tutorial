#include <stm32f0xx_conf.h>
#include "spi.h"

#define SPI_CHANNEL SPI1

/*
 * IF SPI_CHANNEL = SPI1 :
 * - SCK  : PA5
 * - MISO : PA6
 * - MOSI : PA7 = SPI2 :
 *
 * IF SPI_CHANNEL = SPI2 :
 * - SCK  : PB13
 * - MISO : PB14
 * - MOSI : PB15
 * 
 * ------------------------
 *
 * SS : PC3
 *
*/ 

uint8_t txbuf[4], rxbuf[4];
uint16_t txbuf16[4], rxbuf16[4];

int main(void)
{
	int i, j;
	
	// Initialise chip select PC3
	csInit();
	
	spiInit(SPI_CHANNEL);
	
	// Enable RXNE flag if 1 byte is received on MISO pin (default : 2 bytes)
	SPI_RxFIFOThresholdConfig(SPI_CHANNEL ,SPI_RxFIFOThreshold_QF);

	for (i = 0; i < 8; i ++)
	{
		for (j = 0; j < 5; j ++)
			txbuf[j] = i * 4 + j;
		
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);
		spiReadWrite(SPI_CHANNEL, rxbuf, txbuf, 4, SPI_BaudRatePrescaler_64);
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_SET);
		
		for (j = 0; j < 4; j ++)
			if (rxbuf[j] != txbuf[j])
				assert_failed(__FILE__, __LINE__);
	}
			
	// Enable RXNE flag if 2 bytes are received on MISO pin
	SPI_RxFIFOThresholdConfig(SPI_CHANNEL ,SPI_RxFIFOThreshold_HF);

	for (i = 0; i < 8; i ++)
	{
		for (j = 0; j < 4; j ++)
		txbuf16[j] = i * 4 + j + (i << 8);
		
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);
		spiReadWrite16(SPI_CHANNEL, rxbuf16, txbuf16, 4, SPI_BaudRatePrescaler_64);
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_SET);
		
		for (j = 0; j < 4; j ++)
			if (rxbuf16[j] != txbuf16[j])
				assert_failed(__FILE__, __LINE__);			
	}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	/* Infinite loop */
	/* Use GDB to fing out why we're here */
	while(1);
}
#endif
