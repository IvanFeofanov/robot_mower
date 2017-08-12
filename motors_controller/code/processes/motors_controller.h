#ifndef MOTORS_CONTROLLER_H
#define MOTORS_CONTROLLER_H

#include <avr/eeprom.h>

#include "utils/pid.h"

#define ABS(val) (val < 0 ? -val : val)

template<
    typename Motors,
    typename Odometers
    >
class MotorsController
{
public:
    static void init()
    {
        load_config();
    }

    static void update()
    {
        // static uint8_t left = 0;
        // static uint8_t right = 0;

        // //left
        // if(Odometers::left_rps((uint16_t*)(&left_real_rps_)){
        //     left_pwm = Mediator::pid[LEFT].update(
        //             ABS(left_set_rps), left_real_rps_);
        // }
        //
        // //right
        // if(Odometers::right_rps((uint16_t*)(&left_real_rps_)){
        //     right_pwm = Mediator::pid[RIGHT].update(
        //             Mediator::set_rps[RIGHT] < 0 ?
        //             -Mediator::set_rps[RIGHT] : Mediator::set_rps[RIGHT],
        //             Mediator::real_rps[RIGHT]);
        // }
        //
        // Motors::set_duty(left, right);
        // Motors::set_dir(left_set_rps_, right_set_rps_);

    }

    static void set_input_rps(int16_t left, int16_t right)
    {
        left_set_rps_ = left;
        right_set_rps_ = right;
    }

    static void input_rps(int16_t* left, int16_t* right)
    {
        *left = left_set_rps_;
        *right = right_set_rps_;
    }

    static void real_rps(int16_t* left, int16_t* right)
    {
        *left = left_real_rps_;
        *right = right_real_rps_;
    }

    static void set_pid(Pid left, Pid right)
    {
        left_pid_ = left;
        right_pid_ = right;
    }

    static void pid(Pid* left, Pid* right)
    {
        *left = left_pid_;
        *right = right_pid_;
    }

private:
    static void save_config()
    {
        // eeprom_update_word(&ee_r_d_gain_x100_, Mediator::pid[RIGHT].d_gain_x100);
        // eeprom_busy_wait();
    }

    static void load_config()
    {
        // Mediator::pid[RIGHT].d_gain_x100 = eeprom_read_word(&ee_r_d_gain_x100_);
        // eeprom_busy_wait();
    }

private:
    static int16_t left_set_rps_;
    static int16_t right_set_rps_;
    static int16_t left_real_rps_;
    static int16_t right_real_rps_;

    static Pid left_pid_;
    static Pid right_pid_;

    static uint16_t EEMEM ee_r_d_gain_x100_;

};

template<typename Motors, typename Odometers>
int16_t MotorsController<Motors, Odometers>::left_set_rps_;

template<typename Motors, typename Odometers>
int16_t MotorsController<Motors, Odometers>::right_set_rps_;

template<typename Motors, typename Odometers>
int16_t MotorsController<Motors, Odometers>::left_real_rps_;

template<typename Motors, typename Odometers>
int16_t MotorsController<Motors, Odometers>::right_real_rps_;

template<typename Motors, typename Odometers>
Pid MotorsController<Motors, Odometers>::left_pid_;

template<typename Motors, typename Odometers>
Pid MotorsController<Motors, Odometers>::right_pid_;

#endif
