#include "utils/atomic.h"

Atomic::Atomic()
{
    prev_state_ = SREG & (1<<7);
    cli();
}

Atomic::~Atomic()
{
    release();
}

void Atomic::release()
{
    SREG |= prev_state_;
}

NonAtomic::NonAtomic()
{
    prev_state_ = SREG & (1<<7);
    sei();
}

NonAtomic::~NonAtomic()
{
    release();
}

void NonAtomic::release()
{
    SREG |= prev_state_;
}


