#include <stm32f0xx_conf.h>
#include "eeprom.h"

/*
 * SPI_CHANNEL = SPI2 :
 * - SCK  : PB13
 * - MISO : PB14
 * - MOSI : PB15
 * 
 * ------------------------
 *
 * SS : PC10
 *
*/

/*
	This program writes those following words "Zywiec, Tyskie i Wyborowa",
	(which are 2 polish beers and 1 polish Vodka) into the EEPROM from the adress 0xE.
	Datas are set like this :
	
	0x000E : 'Z'
	0x000F : 'y'
	===========
	0x0010 : 'w'
	0x0011 : 'i'
	0x0012 : 'e'
	0x0013 : 'c'
	0x0014 : ','
	0x0015 : ' '
	0x0016 : 'T'
	0x0017 : 'y'
	0x0018 : 's'
	0x0019 : 'k'
	0x001A : 'i'
	0x001B : 'e'
	0x001C : ' '
	0x001D : 'i'
	0x001E : ' '
	0x001F : 'W'
	============
	0x0020 : 'y'
	0x0021 : 'b'
	0x0022 : 'o'
	0x0023 : 'r'
	0x0024 : 'o'
	0x0025 : 'w'
	0x0026 : 'a'
	
	Each 16 bytes page are represented between "==========".
	
	Once those datas are written into the EEPROM, the program
	retreive them from the EEPROM.
	
	If you want to erase the EEPROM, just uncomment the line
	eempromWriteAll(0) at the end of the main function.
*/

int main(void)
{
	// To write
	int8_t send[] = "Zywiec, Tyskie i Wyborowa";
	uint16_t offsetWrite = 0xE;
	
	// To read
	uint8_t recp[25];
	uint8_t offsetRead = offsetWrite;
	
	// EEPROM initialisation
	eepromInit();
	
	// EEPROM Write
	// sizeof(send) - 1 because of the \0 at the end of string
	eepromWrite((uint8_t*) &send, sizeof(send) - 1, offsetWrite);
	
	// EEPROM Read
	eepromRead(recp, sizeof(recp), offsetRead);
	
	// EEPROM Erase
	//eepromWriteAll(0);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	/* Infinite loop */
	/* Use GDB to fing out why we're here */
	while(1);
}
#endif
