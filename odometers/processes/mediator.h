#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <stdint.h>

#include "../utils/pid.h"

class Mediator
{
public:
    enum { LEFT_MOTOR, RIGHT_MOTOR };

    static inline void init()
    {
        set_rps[LEFT_MOTOR] =  0;
        set_rps[RIGHT_MOTOR] = 0;

        real_rps[LEFT_MOTOR] = 0;
        real_rps[RIGHT_MOTOR] = 0;
    }

    static Pid pid[2];
    static int16_t set_rps[2];
    static int16_t real_rps[2];
    static uint32_t counter[2];
};

Pid Mediator::pid[2];
int16_t Mediator::set_rps[2];
int16_t Mediator::real_rps[2];
uint32_t Mediator::counter[2];

#endif
