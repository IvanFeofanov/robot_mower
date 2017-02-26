#include "pid.h"

Pid::Pid()
{
    i_max       = 7000.0;
    i_min       = -7000.0;
    i_gain      = 0.008;
    i_state_    = 0.0;

    p_gain      = 1.0;
    d_gain      = 0.0;
}

Pid::Pid(float i_max, float i_min, float p, float i, float d)
{
    i_max       = i_max;
    i_min       = i_min;
    i_gain      = i;
    i_state_    = 0.0;

    p_gain      = p;
    d_gain      = d;
}


float Pid::update(float x, float y)
{
    float error = x - y;
    error *= 70;

    float p_value = p_gain * error;

    if(i_state_ + error > i_max){
        i_state_ = i_max;
    }else if(i_state_ < i_min){
        i_state_ = i_min;
    }else{
        i_state_ += error;
    }

    float i_value = i_gain * i_state_;

    float summ = p_value + i_value;

    return summ;
}
