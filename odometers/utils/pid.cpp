#include "pid.h"

Pid::Pid()
{
    i_state_    = 0;
    i_max_      = 50;
    i_min_      = -50;
    i_gain_     = 10;

    p_gain_     = 5;
    d_gain_     = 0;

    max_value_  = 255;
    min_value_  = 0;
}

Pid::Pid(float i_max, float i_min, float p, float i, float d)
{
    i_state_    = 0;
    i_max_      = i_max;
    i_min_      = i_min;
    i_gain_     = i;

    p_gain_     = p;
    d_gain_     = d;

    max_value_  = 255;
    min_value_  = 0;
}


float Pid::update(float x, float y)
{
    float error = x - y;

    float p_value = p_gain_ * error;

    i_state_ += error;
    if(i_state_ > i_max_){
        i_state_ = i_max_;
    }else if(i_state_ < i_min_){
        i_state_ = i_min_;
    }

    float i_value = i_gain_ * i_state_;

    float summ = p_value + i_value;

    if(summ >= max_value_)
        return max_value_;
    else if(summ <= min_value_)
        return min_value_;

    return summ;
}
