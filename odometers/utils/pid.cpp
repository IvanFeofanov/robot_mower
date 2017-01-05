#include "pid.h"

Pid::Pid()
{
    i_max       = 50;
    i_min       = -50;
    i_gain      = 10;
    i_state_    = 0;

    p_gain      = 5;
    d_gain      = 0;

    max_value_  = 255;
    min_value_  = 0;
}

Pid::Pid(float i_max, float i_min, float p, float i, float d)
{
    i_max       = i_max;
    i_min       = i_min;
    i_gain      = i;
    i_state_    = 0;

    p_gain      = p;
    d_gain      = d;

    max_value_  = 255;
    min_value_  = 0;
}


float Pid::update(float x, float y)
{
    float error = x - y;

    float p_value = p_gain * error;

    i_state_ += error;
    if(i_state_ > i_max){
        i_state_ = i_max;
    }else if(i_state_ < i_min){
        i_state_ = i_min;
    }

    float i_value = i_gain * i_state_;

    float summ = p_value + i_value;

    if(summ >= max_value_)
        return max_value_;
    else if(summ <= min_value_)
        return min_value_;

    return summ;
}
