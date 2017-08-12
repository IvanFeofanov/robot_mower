#ifndef SPI_H
#define SPI_H

#include <stdint.h>

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
}

enum SpiBitOrder
{
    SPI_LSB_FIRST,
    SPI_MSB_FIRST
};

enum SpiDivider
{
    SPI_DIV_2,
    SPI_DIV_4,
    SPI_DIV_8,
    SPI_DIV_16,
    SPI_DIV_32,
    SPI_DIV_64,
    SPI_DIV_128
};

enum SpiMode
{
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3
};

typedef void (*InPtr)(void);

template<
    SpiBitOrder BIT_ORDER,
    SpiMode     MODE
    >
class SpiSlaveT
{
public:

public:
    static inline void init()
    {
        SPCR = 0;
        SPSR &= ~(1<<SPI2X);

        if(BIT_ORDER == SPI_LSB_FIRST){
            SPCR |= (1<<DORD);
        }

        switch(MODE)
        {
        case SPI_MODE0:
            //nop
            break;
        case SPI_MODE1:
            SPCR |= (1<<CPHA);
            break;
        case SPI_MODE2:
            SPCR |= (1<<CPOL);
            break;
        case SPI_MODE3:
            SPCR |= (1<<CPOL) | (1<<CPHA);
            break;
        }

        DDRB |= (1<<4); //MISO as output
    }

    static void enable()
    {
        SPCR |= (1<<SPE) | (1<<SPIE);
    }

    static void disable()
    {
        SPCR &= ~((1<<SPE) | (1<<SPIE));
    }

    static inline bool is_collision()
    {
        return (SPSR & (1<<WCOL));
    }

    static inline void set_data(uint8_t data)
    {
        SPDR = data;
    }

    static inline uint8_t get_data()
    {
        return SPDR;
    }

    static void attach_handler(InPtr handler_ptr)
    {
        handler_ptr_ = handler_ptr;
    }

public:
    static inline void interrupt_handle()
    {
        handler_ptr_();
    }

private:
    static InPtr handler_ptr_;
};

template<
    SpiBitOrder BIT_ORDER,
    SpiMode     MODE
    >
InPtr SpiSlaveT<BIT_ORDER, MODE>::handler_ptr_;

typedef SpiSlaveT<SPI_MSB_FIRST, SPI_MODE0> SpiSlave;


#endif
