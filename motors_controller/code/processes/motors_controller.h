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
        load_configs();
        save_configs();

        left_set_rps_ = 0;
        right_set_rps_ = 0;
        left_real_rps_ = 0;
        right_real_rps_ = 0;

        left_pid_ = Pid();
        right_pid_ = Pid();
    }

    static void update()
    {
        static uint8_t left = 0;
        static uint8_t right = 0;

        //left
        if(Odometers::left_rps((uint16_t*)(&left_real_rps_))){
            left = left_pid_.update(ABS(left_set_rps_), left_real_rps_);
        }

        //right
        if(Odometers::right_rps((uint16_t*)(&right_real_rps_))){
            right = right_pid_.update(ABS(right_set_rps_), right_real_rps_);
        }

        Motors::set_duty(left, right);
        Motors::set_dir(left_set_rps_, right_set_rps_);
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

    static void get_pid_configs(PidConfig** left, PidConfig** right)
    {
        *left = left_pid_.get_pid_config_ptr();
        *right = right_pid_.get_pid_config_ptr();
    }

    static void save_configs()
    {
        eeprom_busy_wait();
        eeprom_update_block((&ee_pid_config_),
                (void*)(left_pid_.get_pid_config_ptr()),
                sizeof(PidConfig));
        eeprom_busy_wait();
        eeprom_update_block((&ee_pid_config_) + sizeof(PidConfig),
                (void*)(right_pid_.get_pid_config_ptr()),
                sizeof(PidConfig));
    }

    static void load_configs()
    {
        eeprom_busy_wait();
        eeprom_read_block((void*)(left_pid_.get_pid_config_ptr()),
                (&ee_pid_config_),
                sizeof(PidConfig));
        eeprom_busy_wait();
        eeprom_read_block((void*)(right_pid_.get_pid_config_ptr()),
                (&ee_pid_config_) + sizeof(PidConfig),
                sizeof(PidConfig));
    }

private:
    static int16_t left_set_rps_;
    static int16_t right_set_rps_;
    static int16_t left_real_rps_;
    static int16_t right_real_rps_;

    static Pid left_pid_;
    static Pid right_pid_;

    static uint8_t EEMEM ee_pid_config_[sizeof(PidConfig) * 2];

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


template<typename Motors, typename Odometers>
uint8_t MotorsController<Motors, Odometers>::ee_pid_config_[sizeof(PidConfig) * 2] EEMEM;


#endif
