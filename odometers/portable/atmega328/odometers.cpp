#include "odometers.h"

extern volatile uint16_t Odometers::left_counter_;
extern volatile uint16_t Odometers::right_counter_;

ISR(INT0_vect)
{
    Odometers::odometerInterrupt();
}
