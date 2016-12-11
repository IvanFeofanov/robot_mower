#include "pid.h"

Pid::Pid() :
    i_state_(0),
    i_max_(50),
    i_min_(-50),
    p_gain_(5),
    i_gain_(10),
    d_gain_(0)
{
}

double Pid::update(double x, double y)
{
    double error = x - y;

    double p_value = p_gain_ * error;

    i_state_ += error;
    if(i_state_ > i_max_){
        i_state_ = i_max_;
    }else if(i_state_ < i_min_){
        i_state_ = i_min_;
    }

    double i_value = i_gain_ * i_state_;

    double summ = p_value + i_value;

    return summ;
}
