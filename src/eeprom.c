#include <eeprom.h>
#include <spi.h>

#define WIP(x) ((x) & 1)

void eepromInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// SPI initialisation
	spiInit(EEPROM_SPI);
	SPI_RxFIFOThresholdConfig(EEPROM_SPI, SPI_RxFIFOThreshold_QF);
	
	// CSS initialisation
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = EEPROM_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_Init(EEPROM_PORT, &GPIO_InitStructure);
	
	// Set CSS at high level
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_SET);
}

uint8_t eepromReadStatus(void)
{
	uint8_t cmd [] = {cmdRDSR, 0xff};
	uint8_t res[2];

	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_RESET);
	spiReadWrite(EEPROM_SPI, res, cmd, 2, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_SET);

	return res[1];
}

void eepromWriteEnable(void)
{
	uint8_t cmd = cmdWREN;
	
	while(WIP(eepromReadStatus()));
	
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_RESET);
	spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_SET);
}

void eepromWriteDisable(void)
{
	uint8_t cmd = cmdWRDI;
	
	while(WIP(eepromReadStatus()));
	
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_RESET);
	spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_SET);
}

void eepromWriteStatus(uint8_t status)
{
	uint8_t cmd = cmdWRSR;
	
	while(WIP(eepromReadStatus()));
	
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_RESET);
	spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);
	spiReadWrite(EEPROM_SPI, 0, &status, 1, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_SET);
}

int eepromWrite(uint8_t *buf, uint16_t cnt, uint16_t offset)
{
	uint8_t cmd = cmdWRITE;
	
	// Check if end of memory is not reached
	const uint16_t memorySize = 0x1000; // 16 Kbit = 2 Kbytes = 2048 bytes = 0x1000 bytes
	const uint16_t lastMemoryAdress = memorySize - 1;
	uint16_t lastAskedAdress = offset +	cnt - 1;
	
	const uint16_t pageSize = 0x10; // 16 bytes
	const uint16_t pageMask = pageSize - 1;

	uint16_t currentOffset = offset;
	uint16_t swappedCurrentOffset;
	
	uint16_t bufferOffset = 0;
	
	uint16_t memoryWrap = 0;
	
	// If last asked adress is higher than last memory adress, no data write
	if (lastAskedAdress > lastMemoryAdress)
	{
		memoryWrap = lastAskedAdress - lastMemoryAdress;
	}
	else
	{
		uint8_t anotherPageNeeded;
		
		do
		{
			// Number of left bytes to write
			uint16_t nbLeftBytesToWrite = lastAskedAdress - currentOffset + 1;
			
			// Number of bytes available in current page
			uint16_t currentOffsetAndPageMask = currentOffset & pageMask;
			uint16_t nbBytesAvailableInCurrentPage = pageMask - currentOffsetAndPageMask + 1;
			
			// Number of bytes to write in this page
			uint16_t nbBytesToWriteInThisPage;
			
			// Compute if all datas to write fit in current page	
			if (nbLeftBytesToWrite > nbBytesAvailableInCurrentPage)
			{
				anotherPageNeeded = 1;
				nbBytesToWriteInThisPage = nbBytesAvailableInCurrentPage;
			}
			else
			{
				anotherPageNeeded = 0;
				nbBytesToWriteInThisPage = nbLeftBytesToWrite;
			}
			
			// Enable write
			eepromWriteEnable();
				
			// Reset CS
			GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_RESET);
		
			// Send WRITE command
			spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);
		
			/* WARNING
			 * By default, on SPI 16 bits, STM32 send LSByte first.
			 * EEPROM want to receave MSByte first.
			 * There is probably and option to set properly the SPI peripheral.
			 * I didn't find it, so I swapped "by hand" LSByte and MSByte.
			 * If you find the proper option or if you have a better idea, don't hesitate !
			*/
			swappedCurrentOffset = swap_uint16(currentOffset);
			
			// Send adress
			SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_HF);
			spiReadWrite16(EEPROM_SPI, 0, &swappedCurrentOffset, 1, EEPROM_SPEED);
			SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
		
			// Send datas
			spiReadWrite(EEPROM_SPI, 0, buf + bufferOffset, nbBytesToWriteInThisPage, EEPROM_SPEED);
		
			// Set CS
			GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_SET);
			
			bufferOffset += nbBytesToWriteInThisPage;
			currentOffset += nbBytesToWriteInThisPage;
		}
		while(anotherPageNeeded);

		}

	return memoryWrap;
}

int eepromRead(uint8_t *buf, uint16_t cnt, uint16_t offset)
{
	// Check if end of memory is not reached
	uint16_t lastMemoryAdress = 0x7FF;
	uint16_t lastAskedAdress = offset +	cnt - 1;
	
	// If last asked adress is higher than last memory adress, no data read
	uint16_t memoryWrap = 0;
	
	if (lastAskedAdress > lastMemoryAdress)
	{
		memoryWrap = lastAskedAdress - lastMemoryAdress;
	}
	else
	{
		uint8_t cmd = cmdREAD;
		
		/* WARNING
		 * By default, on SPI 16 bits, STM32 send LSByte first.
		 * EEPROM want to receave MSByte first.
		 * There is probably and option to set properly the SPI peripheral.
		 * I didn't find it, so I swapped "by hand" LSByte and MSByte.
		 * If you find the proper option or if you have a better idea, don't hesitate !
		 */
		uint16_t swappedOffset = swap_uint16(offset);
		
		while(WIP(eepromReadStatus()));

		GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_RESET);
		
		// Send READ command
		spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);
		
		// Send address
		SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_HF);
		spiReadWrite16(EEPROM_SPI, 0, &swappedOffset, 1, EEPROM_SPEED);
		SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
	
		// Read datas
		spiReadWrite(EEPROM_SPI, buf, 0, cnt, EEPROM_SPEED);
	
		GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, Bit_SET);
	}
	
	return memoryWrap;
}

void eepromWriteAll(int8_t data)
{
	const uint16_t memorySize = 0x1000; // 16 Kbit = 2 Kbytes = 2048 bytes = 0x1000 bytes
	
	eepromWrite((uint8_t*) &data, memorySize, 0);
}

uint16_t swap_uint16(uint16_t val)
{
	return (val << 8) | (val >> 8);
}
