#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef int16_t Ftype;

/*
 * Для повышения точности вычисления без использования чисел с плавaющей точкой
 * коэфф. регулятора задаются умноженными на 100. Переменные использующие данный
 * формат помечены суффиксом _x100.
 */

class Pid
{
public:
    Pid();
    Ftype update(uint16_t set_rps, uint16_t real_rps);

    int16_t i_max;
    int16_t i_min;
    Ftype i_gain_x100;
    Ftype p_gain_x100;
    Ftype d_gain_x100;

private:
    int16_t i_state_;
    int16_t max_value_;
    int16_t min_value_;
};

#endif
