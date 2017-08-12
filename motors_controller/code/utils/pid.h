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
    // Ftype i_max;
    // Ftype i_min;
    // Ftype i_gain_x100;
    // Ftype p_gain_x100;
    // Ftype d_gain_x100;
    //
    // Ftype max_value;
    // Ftype min_value;
};

class Pid
{
public:
    Pid();
    Ftype update(Ftype set_rps, Ftype real_rps);
    // void set_config(PidConfig* config);
    //
private:
    // PidConfig config_;
    // int16_t i_state_;
};

#endif
