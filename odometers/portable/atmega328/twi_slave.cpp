#include "twi_slave.h"

ISR(TWI_vect)
{
    Twi::interruptHandler();
}
