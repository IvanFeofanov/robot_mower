#ifndef DISABLE_INTERRUPT_H
#define DISABLE_INTERRUPT_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
}

#define I (7)

class DisableInterrupt
{
public:
    DisableInterrupt()
    {
        is_interrupt_enable_ = (SREG & (1<<I));
        cli();
    }

    ~DisableInterrupt()
    {
        if(is_interrupt_enable_)
            sei();
    }

private:
    bool is_interrupt_enable_;
};


#endif
