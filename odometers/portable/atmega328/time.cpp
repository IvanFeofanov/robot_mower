#include "time.h"

extern volatile uint32_t Time::timer_ticks_;
extern volatile uint32_t Time::overflow_ms_;

ISR(TIMER0_OVF_vect)
{
    Time::tickHandler();
}


