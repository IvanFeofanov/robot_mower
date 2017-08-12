#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef int16_t Ftype;

/*
 * Для повышения точности вычисления без использования чисел с плавaющей точкой
 * коэфф. регулятора задаются умноженными на 100. Переменные использующие данный
 * формат помечены суффиксом _x100.
 */

class PidConfig
{
public:
    PidConfig();

public:
    Ftype p_gain_x100;

    Ftype i_gain_x100;
    Ftype i_max;
    Ftype i_min;

    Ftype d_gain_x100;

    Ftype max_value;
    Ftype min_value;
};

class Pid
{
public:
    Pid();
    Ftype update(Ftype input_value, Ftype feedback_value);
    PidConfig* get_pid_config_ptr();

private:
    PidConfig config_;
    Ftype i_state_;
};

#endif
