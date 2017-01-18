#ifndef ODOMETERS_H
#define ODOMETERS_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
}

class Odometers
{
public:
    typedef uint16_t CounterType;

    static void init()
    {
        right_counter_  = 0;
        left_counter_   = 0;
        left_time_      = 0;
        right_time_     = 0;

        // //external interrupt
        // EICRA |= (1<<ISC00);
        // EIMSK |= (1<<INT0);

        TCCR1A = 0;
        TCCR1B = (1<<ICNC1) | (1<<ICES1) | (1<<CS12);
        TIMSK1 = (1<<ICIE1) | (1<<TOIE1);
    }

    static CounterType getLeftCounter()
    {
        CounterType tmp = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            tmp = left_counter_;
        }
        return tmp;
    }

    static CounterType getRightCounter()
    {
        CounterType tmp = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            tmp = right_counter_;
        }
        return tmp;
    }

    static void resetLeftCounter()
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            left_counter_ = 0;
        }
    }

    static void resetRightCounter()
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            right_counter_ = 0;
        }
    }

    static uint16_t getLeftRps()
    {
        uint32_t rps = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            rps = left_time_ * 16; //to us
        }

        return rps == 0 ? 0 : 1000000 / rps;
    }

    static uint16_t getRightRps()
    {
        return 0;
    }

public:
    volatile static void captureInterrupt()
    {
        left_counter_++;
        // right_counter_++;

        left_time_ = (left_time_ + ICR1) / 2;
        TCNT1 = 0;
    }

    volatile static void overflowInterrupt()
    {
        left_time_ = 0;
    }

private:
    volatile static CounterType left_counter_;
    volatile static CounterType right_counter_;
    volatile static uint16_t left_time_;
    volatile static uint16_t right_time_;

};

#endif
