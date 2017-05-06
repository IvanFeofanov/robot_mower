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
    uint16_t        RECEIVE_BUFFER_SIZE,
    uint16_t        TRANSFER_BUFFER_SIZE
    >
class Uart
{
public:
    static void init()
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

    }

    static uint16_t write(const void* buffer, uint16_t length)
    {
        uint16_t empty_space = txAvailable();
        uint16_t len = length < empty_space ? length : empty_space;
        bool is_writen = transfer_buffer_.isEmpty();

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            for(uint16_t i = 0; i < len; i++){
                transfer_buffer_.put(((uint8_t*)buffer)[i]);
            }

            if(is_writen){
                uint8_t b;
                transfer_buffer_.take(&b);
                UDR0 = b;
                UCSR0B |= (1<<UDRIE0); //UDR empty interrupt enable
            }
        }

        return len;
    }

    static int16_t write(uint8_t byte)
    {
        if(txAvailable() == 0){
            return -1;
        }

        bool is_writen = transfer_buffer_.isEmpty();

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if(is_writen){
                UDR0 = byte;
                UCSR0B |= (1<<UDRIE0); //UDR empty interrupt enable
            }else{
                transfer_buffer_.put(byte);
            }
        }

        return byte;
    }

    static uint16_t txAvailable()
    {
        uint16_t empty_space = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            empty_space = TRANSFER_BUFFER_SIZE - transfer_buffer_.count();
        }

        return empty_space;
    }

    static uint16_t rxAvailable()
    {
        uint16_t count = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            count = receive_buffer_.count();
        }

        return count;
    }

    static int16_t read()
    {
        uint8_t value = 0;
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
                    ((uint8_t*)buffer)[len] = value;
                }else{
                    break;
                }
            }
        }

        return len;
    }

    static void rxFlush()
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            receive_buffer_.flush();
        }
    }

    static void txFlush()
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            transfer_buffer_.flush();
        }
    }

public:
    static void receptionComplate()
    {
        receive_buffer_.put(UDR0);
    }

    static void dataRegisterEmpty()
    {
        if(transfer_buffer_.isEmpty()){
            UCSR0B &= ~(1<<UDRIE0); //UDR empty interrupt disable
        }else{
            uint8_t byte;
            transfer_buffer_.take(&byte);
            UDR0 = byte;
        }
    }

    static void transferComplate()
    {
    }

private:
    static RingBuffer<RECEIVE_BUFFER_SIZE, uint8_t> receive_buffer_;
    static RingBuffer<TRANSFER_BUFFER_SIZE, uint8_t> transfer_buffer_;
    static bool is_writen_;
};

template<
    UART_BAUND_RATE BAUND_RATE,
    UART_SPEED_MODE SPEED_MODE,
    uint16_t        RECEIVE_BUFFER_SIZE,
    uint16_t        TRANSFER_BUFFER_SIZE
    >
RingBuffer<RECEIVE_BUFFER_SIZE, uint8_t>
Uart<BAUND_RATE,
    SPEED_MODE,
    RECEIVE_BUFFER_SIZE,
    TRANSFER_BUFFER_SIZE>::receive_buffer_;

template<
    UART_BAUND_RATE BAUND_RATE,
    UART_SPEED_MODE SPEED_MODE,
    uint16_t        RECEIVE_BUFFER_SIZE,
    uint16_t        TRANSFER_BUFFER_SIZE
    >
RingBuffer<TRANSFER_BUFFER_SIZE, uint8_t>
Uart<BAUND_RATE,
    SPEED_MODE,
    RECEIVE_BUFFER_SIZE,
    TRANSFER_BUFFER_SIZE>::transfer_buffer_;

typedef Uart<UBR_9600, UART_NORMAL_SPEED_MODE, 128, 128> Serial;

#endif
