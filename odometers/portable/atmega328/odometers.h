#ifndef ODOMETERS_H
#define ODOMETERS_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
}

#include "disable_interrupt.h"


class Odometers
{
public:
    static void init()
    {
        right_counter_ = 0;
        left_counter_ = 0;

        //external interrupt
        EICRA |= (1<<ISC00);
        EIMSK |= (1<<INT0);
    }

    static uint16_t getLeft()
    {
        DisableInterrupt di;
        uint16_t tmp = left_counter_;
        left_counter_ = 0;

        return tmp;
    }

    static uint16_t getRight()
    {
        DisableInterrupt di;
        uint16_t tmp = right_counter_;
        right_counter_ = 0;

        return tmp;
    }

    static void resetLeft()
    {
        DisableInterrupt di;
        left_counter_ = 0;
    }

    static void resetRight()
    {
        DisableInterrupt di;
        right_counter_ = 0;
    }

public:
    volatile static void odometerInterrupt()
    {
        left_counter_++;
        right_counter_++;
    }

private:
    volatile static uint16_t left_counter_;
    volatile static uint16_t right_counter_;

};

#endif
