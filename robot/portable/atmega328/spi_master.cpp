#include "spi_master.h"

ISR(SPI_STC_vect)
{
    SpiMaster::interrupt_handler();
}
