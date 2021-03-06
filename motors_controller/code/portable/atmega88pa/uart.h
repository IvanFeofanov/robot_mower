#ifndef UART_H
#define UART_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
}

#include "ring_buffer.h"


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
};

template<
    UART_BAUND_RATE BAUND_RATE,
    UART_SPEED_MODE SPEED_MODE,
    uint16_t        RECEIVE_BUFFER_SIZE
    >
class Uart
{
public:
    static inline void init()
    {
        //baund rate
        uint16_t ubrr = 0;

        if(SPEED_MODE == UART_NORMAL_SPEED_MODE){
            UCSR0A &= ~(1<<U2X0);
            ubrr = F_CPU / (16 * BAUND_RATE) - 1;
        }else{
            UCSR0A |= (1<<U2X0);
            ubrr = F_CPU / (8 * BAUND_RATE) - 1;
        }

        UBRR0H = (uint8_t)(ubrr >> 8);
        UBRR0L = ubrr;

        //receiver and transmitter enable, TX and RX complate interrupt enable
        UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<TXCIE0) | (1<<RXCIE0);

        //Asynchronous USART, Parity Mode - disabled, 1 - stop bit,
        //8 - bit character Size
        UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);

        transfer_buffer_ptr_    = 0;
        transfer_buffer_index_  = 0;
    }

    static bool write(const void* buffer, uint16_t length)
    {
        if(length == 0 || !isWriten())
            return false;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            transfer_buffer_ptr_     = (char*)buffer;
            transfer_buffer_length_  = length;

            UDR0 = transfer_buffer_ptr_[transfer_buffer_index_];

            UCSR0B |= (1<<UDRIE0); //UDR empty interrupt enable

        }

        return true;
    }

    static bool isWriten()
    {
        bool is_writen = false;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            is_writen = transfer_buffer_length_ == 0;
        }

        return is_writen;
    }

    static uint16_t available()
    {
        uint16_t count = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            count = receive_buffer_.count();
        }

        return count;
    }

    static char read()
    {
        char value = 0;
        bool is_ok = false;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            is_ok = receive_buffer_.take(&value);
        }

        if(is_ok)
            return value;
        return -1;
    }

    static uint16_t read(void* buffer, uint16_t length)
    {
        uint16_t len = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            char value;
            for(len = 0; len < length; len++){
                if(receive_buffer_.take(&value)){
                    ((char*)buffer)[len] = value;
                }else{
                    break;
                }
            }
        }

        return len;
    }

    static void flush()
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            receive_buffer_.flush();
        }
    }

public:
    static void receptionComplate()
    {
        receive_buffer_.put(UDR0);
    }

    static void dataRegisterEmpty()
    {
        transfer_buffer_index_++;

        if(transfer_buffer_index_ == transfer_buffer_length_){
            UCSR0B &= ~(1<<UDRIE0); //UDR empty interrupt disable
        }else{
            UDR0 = transfer_buffer_ptr_[transfer_buffer_index_];
        }
    }

    static void transferComplate()
    {
        transfer_buffer_ptr_    = 0;
        transfer_buffer_index_  = 0;
        transfer_buffer_length_ = 0;
    }

private:
    static RingBuffer<RECEIVE_BUFFER_SIZE, char> receive_buffer_;
    static char*    transfer_buffer_ptr_;
    static uint16_t transfer_buffer_index_;
    static uint16_t transfer_buffer_length_;

};

template<
    UART_BAUND_RATE BAUND_RATE,
    UART_SPEED_MODE SPEED_MODE,
    uint16_t        RECEIVE_BUFFER_SIZE
    >
RingBuffer<RECEIVE_BUFFER_SIZE, char> Uart<BAUND_RATE, SPEED_MODE, RECEIVE_BUFFER_SIZE>::receive_buffer_;

template<
    UART_BAUND_RATE BAUND_RATE,
    UART_SPEED_MODE SPEED_MODE,
    uint16_t        RECEIVE_BUFFER_SIZE
    >
char* Uart<BAUND_RATE, SPEED_MODE, RECEIVE_BUFFER_SIZE>::transfer_buffer_ptr_;

template<
    UART_BAUND_RATE BAUND_RATE,
    UART_SPEED_MODE SPEED_MODE,
    uint16_t        RECEIVE_BUFFER_SIZE
    >
uint16_t Uart<BAUND_RATE, SPEED_MODE, RECEIVE_BUFFER_SIZE>::transfer_buffer_index_;

template<
    UART_BAUND_RATE BAUND_RATE,
    UART_SPEED_MODE SPEED_MODE,
    uint16_t        RECEIVE_BUFFER_SIZE
    >
uint16_t Uart<BAUND_RATE, SPEED_MODE, RECEIVE_BUFFER_SIZE>::transfer_buffer_length_;


typedef Uart<UBR_9600, UART_NORMAL_SPEED_MODE, 32> Serial;


#endif
