#ifndef MOTORS_CONTROLLER_H
#define MOTORS_CONTROLLER_H

#include <avr/eeprom.h>

#include "mediator.h"

template<
    typename Motors,
    typename Odometers
    >
class MotorsController
{
public:
    enum {  LEFT = Mediator::LEFT_MOTOR,
            RIGHT = Mediator::RIGHT_MOTOR };

    void init()
    {
        last_time_ = 0;
        loadPidSettings();
    }

    void run()
    {
        // PORTB |= (1<<5);
        // Motors::setPwm(100, 100);
        // Motors::setDirection(1, 1);
        //
        uint16_t rps;
        static uint8_t left_pwm = 0;
        static uint8_t right_pwm = 0;

        //left
        if(Odometers::leftRps(&rps)){
            Mediator::real_rps[LEFT] = rps;
            Mediator::counter[LEFT] = Odometers::getLeftCounter();
            left_pwm = Mediator::pid[LEFT].update(
                    Mediator::set_rps[LEFT] < 0 ?
                    -Mediator::set_rps[LEFT] : Mediator::set_rps[LEFT],
                    Mediator::real_rps[LEFT]);
        }

        //right
        if(Odometers::rightRps(&rps)){
            Mediator::real_rps[RIGHT] = rps;
            Mediator::counter[RIGHT] = Odometers::getRightCounter();
            right_pwm = Mediator::pid[RIGHT].update(
                    Mediator::set_rps[RIGHT] < 0 ?
                    -Mediator::set_rps[RIGHT] : Mediator::set_rps[RIGHT],
                    Mediator::real_rps[RIGHT]);
        }

        Motors::setPwm(left_pwm, right_pwm);
        Motors::setDirection(Mediator::set_rps[LEFT],
                             Mediator::set_rps[RIGHT]);


        //save pid settings
        if(Mediator::save_pid_settings_flag){
            savePidSettings();
            Mediator::save_pid_settings_flag = false;
        }
    }

private:
    void savePidSettings()
    {
        eeprom_busy_wait();
        eeprom_update_word(&ee_l_p_gain_x100_, Mediator::pid[LEFT].p_gain_x100);
        eeprom_busy_wait();
        eeprom_update_word(&ee_l_i_gain_x100_, Mediator::pid[LEFT].i_gain_x100);
        eeprom_busy_wait();
        eeprom_update_word(&ee_l_d_gain_x100_, Mediator::pid[LEFT].d_gain_x100);
        eeprom_busy_wait();

        eeprom_update_word(&ee_r_p_gain_x100_, Mediator::pid[RIGHT].p_gain_x100);
        eeprom_busy_wait();
        eeprom_update_word(&ee_r_i_gain_x100_, Mediator::pid[RIGHT].i_gain_x100);
        eeprom_busy_wait();
        eeprom_update_word(&ee_r_d_gain_x100_, Mediator::pid[RIGHT].d_gain_x100);
        eeprom_busy_wait();
    }

    void loadPidSettings()
    {
        eeprom_busy_wait();
        Mediator::pid[LEFT].p_gain_x100 = eeprom_read_word(&ee_l_p_gain_x100_);
        eeprom_busy_wait();
        Mediator::pid[LEFT].i_gain_x100 = eeprom_read_word(&ee_l_i_gain_x100_);
        eeprom_busy_wait();
        Mediator::pid[LEFT].d_gain_x100 = eeprom_read_word(&ee_l_d_gain_x100_);
        eeprom_busy_wait();

        Mediator::pid[RIGHT].p_gain_x100 = eeprom_read_word(&ee_r_p_gain_x100_);
        eeprom_busy_wait();
        Mediator::pid[RIGHT].i_gain_x100 = eeprom_read_word(&ee_r_i_gain_x100_);
        eeprom_busy_wait();
        Mediator::pid[RIGHT].d_gain_x100 = eeprom_read_word(&ee_r_d_gain_x100_);
        eeprom_busy_wait();
    }

private:
    uint32_t last_time_;

    static uint16_t EEMEM ee_l_p_gain_x100_;
    static uint16_t EEMEM ee_l_i_gain_x100_;
    static uint16_t EEMEM ee_l_d_gain_x100_;

    static uint16_t EEMEM ee_r_p_gain_x100_;
    static uint16_t EEMEM ee_r_i_gain_x100_;
    static uint16_t EEMEM ee_r_d_gain_x100_;

};

template<typename Motors, typename Odometers>
uint16_t EEMEM MotorsController<Motors, Odometers>::ee_l_p_gain_x100_ = 200;

template<typename Motors, typename Odometers>
uint16_t EEMEM MotorsController<Motors, Odometers>::ee_l_i_gain_x100_ = 100;

template<typename Motors, typename Odometers>
uint16_t EEMEM MotorsController<Motors, Odometers>::ee_l_d_gain_x100_ = 0;

template<typename Motors, typename Odometers>
uint16_t EEMEM MotorsController<Motors, Odometers>::ee_r_p_gain_x100_ = 200;

template<typename Motors, typename Odometers>
uint16_t EEMEM MotorsController<Motors, Odometers>::ee_r_i_gain_x100_ = 100;

template<typename Motors, typename Odometers>
uint16_t EEMEM MotorsController<Motors, Odometers>::ee_r_d_gain_x100_ = 0;


#endif
