#include "pid.h"

Pid::Pid()
{
    i_max       = 200;
    i_min       = -200;
    i_gain_x100 = 0;
    i_state_    = 0;

    p_gain_x100 = 0;
    d_gain_x100 = 0;

    max_value_  = 255;
    min_value_  = 0;
}

Ftype Pid::update(uint16_t set_rps, uint16_t real_rps)
{
    Ftype error = set_rps - real_rps;

    Ftype p_value = p_gain_x100 * error;

    i_state_ += error;
    if(i_state_ > i_max){
        i_state_ = i_max;
    }else if(i_state_ < i_min){
        i_state_ = i_min;
    }

    Ftype i_value = i_gain_x100 * i_state_;

    Ftype summ = (p_value + i_value) / 100;

    if(summ >= max_value_)
        return max_value_;
    else if(summ <= min_value_)
        return min_value_;

    return summ;
}
