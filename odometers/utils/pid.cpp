#include "pid.h"

Pid::Pid()
{
    i_max       = 200;
    i_min       = -200;
    i_gain      = 1;
    i_state_    = 0;

    p_gain      = 2;
    d_gain      = 0;

    max_value_  = 255;
    min_value_  = 0;
}

Ftype Pid::update(uint16_t set_rps, uint16_t real_rps)
{
    Ftype error = set_rps - real_rps;

    Ftype p_value = p_gain * error;

    i_state_ += error;
    if(i_state_ > i_max){
        i_state_ = i_max;
    }else if(i_state_ < i_min){
        i_state_ = i_min;
    }

    Ftype i_value = i_gain * i_state_;

    Ftype summ = p_value + i_value;

    if(summ >= max_value_)
        return max_value_;
    else if(summ <= min_value_)
        return min_value_;

    return summ;
}
