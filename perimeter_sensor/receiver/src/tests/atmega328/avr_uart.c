#include "avr_uart.h"
#include <avr/io.h>

#define BAUND 57600
#define UBRR_VAL ((F_CPU / (16UL * BAUND)) - 1)

//initialize the UART
void avr_uart_init()
{
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
    UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
    UBRR0H = (UBRR_VAL >> 8);
    UBRR0L = UBRR_VAL;
}

void avr_uart_send_byte(char tx_byte)
{
    //wait to be able to transmit
    while(!(UCSR0A & (1<<UDRE0)));

    UDR0 = tx_byte;
}

int16_t avr_uart_receive_byte()
{
    if(UCSR0A & (1<<RXC0)){
        return UDR0;
    }else{
        return -1;
    }
}
