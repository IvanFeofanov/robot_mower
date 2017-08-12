#include "pid.h"

PidConfig::PidConfig() :
    p_gain_x100(200),
    i_gain_x100(100),
    i_max(200),
    i_min(-200),
    d_gain_x100(0),
    max_value(255),
    min_value(0)
{
}

Pid::Pid() :
    config_(),
    i_state_(0)
{
}


Ftype Pid::update(Ftype set_rps, Ftype real_rps)
{
    Ftype error = set_rps - real_rps;

    int32_t p_value = config_.p_gain_x100 * error;

    i_state_ += error;
    if(i_state_ > config_.i_max){
        i_state_ = config_.i_max;
    }else if(i_state_ < config_.i_min){
        i_state_ = config_.i_min;
    }

    int32_t i_value = config_.i_gain_x100 * i_state_;

    Ftype summ = (p_value + i_value) / 100;

    if(summ >= config_.max_value)
        return config_.max_value;
    else if(summ <= config_.min_value)
        return config_.min_value;

    return summ;
}

PidConfig* Pid::get_pid_config_ptr()
{
    return &config_;
}
