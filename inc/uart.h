#include <stm32f0xx_conf.h>

// Open UART, configure baudrate and the Tx/Rx
int uart_open(USART_TypeDef* USARTx, uint32_t baud, uint32_t flags);

// Close UART
int uart_close(USART_TypeDef* USARTx);

// Get a character from UART
char uart_getc(USART_TypeDef* USARTx);

// Put a character to UART
int uart_putc(USART_TypeDef* USARTx, char c);
