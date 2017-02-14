#ifndef AVR_UART_H
#define AVR_UART_H

#include <stdint.h>

void avr_uart_init();
void avr_uart_send_byte(char tx_byte);
int16_t avr_uart_receive_byte();

#endif
