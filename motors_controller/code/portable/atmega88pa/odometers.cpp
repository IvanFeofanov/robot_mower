#include "odometers.h"

extern volatile Odometers::CounterType Odometers::left_counter_;
extern volatile Odometers::CounterType Odometers::right_counter_;
extern volatile uint16_t Odometers::left_tc_;
extern volatile bool     Odometers::is_left_tc_overflow_;
extern volatile uint16_t Odometers::left_time_;
extern volatile uint16_t Odometers::right_tc_;
extern volatile bool     Odometers::is_right_tc_overflow_;
extern volatile uint16_t Odometers::right_time_;

ISR(INT0_vect)
{
    Odometers::left_odometer_interrupt();
}

ISR(INT1_vect)
{
    Odometers::right_odometer_interrupt();
}

ISR(TIMER1_COMPA_vect)
{
    Odometers::timer_interrupt();
}
