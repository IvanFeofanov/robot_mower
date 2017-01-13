#include "twi.h"

ISR(TWI_vect)
{
    I2c::interruptHandler();
}
