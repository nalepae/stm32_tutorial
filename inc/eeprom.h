#include <stm32f0xx_conf.h>

#define EEPROM_PORT GPIOC
#define EEPROM_CS GPIO_Pin_10
#define EEPROM_SPI SPI2
#define EEPROM_SPEED SPI_BaudRatePrescaler_64

void eepromInit(void);
void eepromWriteEnable(void);
void eepromWriteDisable(void);
uint8_t eepromReadStatus(void);
void eepromWriteStatus(uint8_t status);
int eepromWrite(uint8_t *buf, uint16_t cnt, uint16_t offset);
int eepromRead(uint8_t *buf, uint16_t cnt, uint16_t offset);

// Write all the EEPROM with data "data"
void eepromWriteAll(int8_t data);

uint16_t swap_uint16(uint16_t val);

enum eepromCMD {	cmdREAD = 0x03, cmdWRITE = 0x02,
									cmdWREN = 0x06, cmdWRDI  = 0x04,
									cmdRDSR = 0x05, cmdWRSR  = 0x01	};
