#include "avr_uart.h"
#include <avr/io.h>

#define BAUND 57600
/* #define UBRR_VAL ((F_CPU / (16UL * BAUND)) - 1) */
#define UBRR_VAL 25

//initialize the UART
void avr_uart_init()
{
    UCSRA = 0;
    UCSRB |= (1<<RXEN) | (1<<TXEN);
    UCSRC |= (1<<UCSZ1) | (1<<UCSZ0) | (1<<URSEL);
    UBRRH = 0; //(UBRR_VAL >> 8);
    UBRRL = 103; //UBRR_VAL;
}

void avr_uart_send_byte(char tx_byte)
{
    //wait to be able to transmit
    while(!(UCSRA & (1<<UDRE)));

    UDR = tx_byte;
}

int16_t avr_uart_receive_byte()
{
    if(UCSRA & (1<<RXC)){
        return UDR;
    }else{
        return -1;
    }
}
