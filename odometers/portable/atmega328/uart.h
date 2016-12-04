#ifndef UART_H
#define UART_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
}

enum UART_BAUND_RATE
{
    UBR_2400 = 2400,
    UBR_9600 = 9600,
    UBR_57600 = 57600,
    UBR_115200 = 115200
};

enum UART_SPEED_MODE
{
    UART_NORMAL_SPEED_MODE,
    UART_DOUBLE_SPEED_MODE
}

//baund rate
// template<UART_BAUND_RATE BAUND_RATE, UART_SPEED_MODE SPEED_MODE>
// struct UartBaundRate{ };
//
// template<UART_BAUND_RATE BAUND_RATE>
// struct UartBaundRate<BAUND_RATE, UART_NORMAL_SPEED_MODE>{
//     enum { ubrr = F_CPU / (16 * BAUND_RATE) - 1 };
//     enum { ubrrh = (uint8_t)(ubrr >> 8) };
//     enum { ubrrl = (uint8_t)(ubrr) };
// };
//
// template<UART_BAUND_RATE BAUND_RATE>
// struct UartBaundRate<BAUND_RATE, UART_DOUBLE_SPEED_MODE>{
//     enum { ubrr = F_CPU / (8 * BAUND_RATE) - 1 };
// };
//

template<
    UART_BAUND_RATE BAUND_RATE,
    UART_SPEED_MODE SPEED_MODE,
    uint16_t        RECEIVE_BUFFER_SIZE
    >
class Uart
{
public:
    static void init()
    {
        //baund rate
        uint16_t ubrr = 0;

        if(SPEED_MODE = UART_NORMAL_SPEED_MODE){
            UCSR0A &= ~(1<<U2X0);
            ubrr = F_CPU / (16 * BAUND_RATE) - 1;
        }else{
            UCSR0A |= (1<<U2X0);
            ubrr = F_CPU / (8 * BAUND_RATE) - 1;
        }

        UBRR0H = (uint8_t)(ubrr >> 8);
        UBRR0L = ubrr;

        //receiver and transmitter enable, TX and RX complate interrupt enable
        UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) | (1<<TXCIE0);

        //Asynchronous USART, Parity Mode - disabled, 1 - stop bit,
        //8 - bit character Size
        UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);

        transfer_buffer_ptr_ = NULL;
        receive_buffer_index_ = 0;
    }

    static uint16_t available()
    {
        cli();
        uint16_t index = receive_buffer_index_;
        sei();
        return index;
    }

    static uint16_t write(void* buffer, uint16_t length)
    {

    }

    // static uint16_t read(void* buffer, uint16_t length);
    static char read()
    {
        // cli();
        // char data = receive_buffer_[receive_buffer_index_];
        // receive_buffer_index_--;
        // sei();

    }

private:
    static char        receive_buffer_[RECEIVE_BUFFER_SIZE];
    static uint16_t    receive_buffer_index_;
    static char*       transfer_buffer_ptr_;

};

typedef Uart<UBR_9600, UART_NORMAL_SPEED_MODE, 128> Uart9600;


#endif
