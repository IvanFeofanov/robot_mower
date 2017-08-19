#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include <stdint.h>

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
}

#include "portable/atmega328/pio.h"

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

template<
    SpiBitOrder BIT_ORDER,
    SpiDivider  DIVIDER,
    SpiMode     MODE,
    typename    MosiPin,
    typename    SckPin,
    typename    CePin
    >
class SpiMasterT
{
public:
    typedef CePin SsPin;

public:
    static inline void init()
    {
        CePin::setOutput();
        MosiPin::setOutput();
        SckPin::setOutput();


        SPCR |= (1<<SPE) | (1<<SPIE) | (1<<MSTR); //enable module and interrupts;
        SPSR &= ~(1<<SPI2X);

        if(BIT_ORDER == SPI_LSB_FIRST){
            SPCR |= (1<<DORD);
        }

        switch(DIVIDER)
        {
        case SPI_DIV_2:
            SPSR |= (1<<SPI2X);
            break;

        case SPI_DIV_4:
            //
            break;

        case SPI_DIV_8:
            SPSR |= (1<<SPI2X);
            SPCR |= (1<<SPR0);
            break;

        case SPI_DIV_16:
            SPCR |= (1<<SPR0);
            break;

        case SPI_DIV_32:
            SPCR |= (1<<SPR1);
            SPSR |= (1<<SPI2X);
            break;

        case SPI_DIV_64:
            SPCR |= (1<<SPR1);
            break;

        case SPI_DIV_128:
            SPCR |= (1<<SPR1);
            SPCR |= (1<<SPR0);
            break;
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

        is_available_ = true;
        len_          = 0;
        index_        = 0;
        buffer_tx_    = 0;
        buffer_rx_    = 0;
    }

    static void transfer(void* buffer_tx, void* buffer_rx, uint16_t len)
    {
        if(!is_available_)
            return;

        buffer_tx_ = static_cast<uint8_t*>(buffer_tx);
        buffer_rx_ = static_cast<uint8_t*>(buffer_rx);
        len_       = len;
        index_     = 0;
        is_available_ = false;

        SPDR = (buffer_tx_ ? buffer_tx_[index_] : 0xff);
    }

    static bool is_available()
    {
        return is_available_;
    }


public:
    static inline void interrupt_handler()
    {
        if(buffer_rx_){
            buffer_rx_[index_] = SPDR;
        }

        index_++;

        if(index_ < len_){
            SPDR = (buffer_tx_ ? buffer_tx_[index_] : 0xff);
        }else{
            is_available_ = true;
        }
    }

private:
    volatile static bool is_available_;

    volatile static uint16_t len_;
    volatile static uint16_t index_;
    static uint8_t* buffer_tx_;
    static uint8_t* buffer_rx_;
};

template<
    SpiBitOrder BIT_ORDER,
    SpiDivider  DIVIDER,
    SpiMode     MODE,
    typename    MosiPin,
    typename    SckPin,
    typename    CePin

    >
volatile bool SpiMasterT<BIT_ORDER, DIVIDER, MODE, MosiPin, SckPin, CePin>::is_available_;

template<
    SpiBitOrder BIT_ORDER,
    SpiDivider  DIVIDER,
    SpiMode     MODE,
    typename    MosiPin,
    typename    SckPin,
    typename    CePin

    >
volatile uint16_t SpiMasterT<BIT_ORDER, DIVIDER, MODE, MosiPin, SckPin, CePin>::len_;

template<
    SpiBitOrder BIT_ORDER,
    SpiDivider  DIVIDER,
    SpiMode     MODE,
    typename    MosiPin,
    typename    SckPin,
    typename    CePin

    >
volatile uint16_t SpiMasterT<BIT_ORDER, DIVIDER, MODE, MosiPin, SckPin, CePin>::index_;

template<
    SpiBitOrder BIT_ORDER,
    SpiDivider  DIVIDER,
    SpiMode     MODE,
    typename    MosiPin,
    typename    SckPin,
    typename    CePin

    >
uint8_t* SpiMasterT<BIT_ORDER, DIVIDER, MODE, MosiPin, SckPin, CePin>::buffer_tx_;

template<
    SpiBitOrder BIT_ORDER,
    SpiDivider  DIVIDER,
    SpiMode     MODE,
    typename    MosiPin,
    typename    SckPin,
    typename    CePin
    >
uint8_t* SpiMasterT<BIT_ORDER, DIVIDER, MODE, MosiPin, SckPin, CePin>::buffer_rx_;

typedef SpiMasterT<SPI_MSB_FIRST, SPI_DIV_4, SPI_MODE0, PioB3, PioB5, PioB2> SpiMaster;

#endif
