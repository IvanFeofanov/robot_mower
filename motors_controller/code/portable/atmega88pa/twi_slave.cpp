#include "twi_slave.h"

ISR(TWI_vect)
{
    Twi::interrupt_handler();
}
