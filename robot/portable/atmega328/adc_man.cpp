#include "adc_man.h"

ISR(ADC_vect)
{
    Adc::interruptHandler();
}
