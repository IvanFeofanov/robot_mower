#include "odometers.h"

extern volatile uint16_t Odometers::left_counter_;
extern volatile uint16_t Odometers::right_counter_;
extern volatile uint16_t Odometers::left_time_;
extern volatile uint16_t Odometers::right_time_;

ISR(TIMER1_CAPT_vect)
{
    Odometers::captureInterrupt();
}

ISR(TIMER1_OVF_vect)
{
    Odometers::overflowInterrupt();
}
