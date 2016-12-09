#ifndef TIME_H
#define TIME_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
}

#include "disable_interrupt.h"

class Time
{
public:
    static void init()
    {
        TCCR0B |= (1<<CS01) | (1<<CS00);
        TIMSK0 |= (1<<TOIE0);
    }

    static uint32_t now()
    {
        DisableInterrupt di();

        uint32_t time_now;

        time_now = overflow_ms_;
        return time_now;
    }

public:
    
    static void tickHandler()
    {
        timer_ticks_ += CLOCK_TICKS_PER_TIMER_OVERFLOW;
        for(; timer_ticks_ > CLOCK_TICKS_PER_MS; timer_ticks_ -= CLOCK_TICKS_PER_MS){
            overflow_ms_++;
        }
    }

private:
    enum { CLOCK_TICKS_PER_MS = (F_CPU / 1000UL) };

    //timer is 8 bits and is prescaled by 64
    enum { CLOCK_TICKS_PER_TIMER_OVERFLOW = (64UL * 256UL) };

    volatile static uint32_t timer_ticks_;
    volatile static uint32_t overflow_ms_;
};

#endif
