#ifndef UTILS_ATOMIC_H
#define UTILS_ATOMIC_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
}

class Atomic
{
public:
    Atomic();
    ~Atomic();

    void release();

private:
    uint8_t prev_state_;
};

class NonAtomic
{
public:
    NonAtomic();
    ~NonAtomic();

    void release();

private:
    uint8_t prev_state_;
};

#endif
