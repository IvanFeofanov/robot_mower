#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef int16_t Ftype;

class Pid
{
public:
    Pid();
    Ftype update(uint16_t set_rps, uint16_t real_rps);

    Ftype i_max;
    Ftype i_min;
    Ftype i_gain;
    Ftype p_gain;
    Ftype d_gain;

private:
    Ftype i_state_;
    Ftype max_value_;
    Ftype min_value_;
};

#endif
