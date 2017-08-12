#include "spi.h"

ISR(SPI_STC_vect)
{
    SpiSlave::interrupt_handle();
}
