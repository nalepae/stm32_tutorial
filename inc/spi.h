#include <stm32f0xx_conf.h>

// Initialise SPI speed
enum speeds {SPI_SLOW, SPI_MEDIUM, SPI_FAST};

void spiInit(SPI_TypeDef* SPIx);

// Read/Write 1 byte
int spiReadWrite(SPI_TypeDef* SPIx, uint8_t *rbuf, const uint8_t *tbuf, int cnt, uint16_t speed);

// Read/Write 2 bytes
int spiReadWrite16(SPI_TypeDef* SPIx, uint16_t *rbuf, const uint16_t *tbuf, int cnt, uint16_t speed);

// Initialise chip select PC03
void csInit(void);
