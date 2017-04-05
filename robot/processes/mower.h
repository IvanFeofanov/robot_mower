#ifndef MOWER_H
#define MOWER_H

#include "map.h"

template<
    typename Motor
    >
class Mower
{
private:
    // must be >= 255
    enum { SETTLING_TIME = 1000 }; //ms
    //time betwen set
    enum { TBS = SETTLING_TIME / 255 };

public:
    static void init()
    {
        is_enable_ = false;
        pwm_ = Motor::MIN_PWM;
        current_pwm_ = Motor::MIN_PWM;
        last_time_ = 0;

        Motor::setPwm(Motor::MIN_PWM);
    }

    static void update()
    {
        if(pwm_ == Motor::MIN_PWM || !is_enable_){ //stop
            Motor::setPwm(Motor::MIN_PWM);
            current_pwm_ = Motor::MIN_PWM;
            return;
        }

        if(Time::now() - last_time_ >= TBS){
            Motor::setPwm(current_pwm_);

            if(current_pwm_ < pwm_){
                current_pwm_ += 1;
            }

            if(current_pwm_ > pwm_){
                current_pwm_ -= 1;
            }

            last_time_ = Time::now();
        }
    }

    static void setSpeed(uint8_t speed)
    {
        pwm_ = speed;
    }

    static inline uint8_t speed()
    {
        return pwm_;
    }

    static void setEnable(bool is_enable)
    {
        is_enable_ = is_enable;
    }

    static inline bool isEnable()
    {
        return is_enable_;
    }

private:
    static bool is_enable_;
    static uint8_t pwm_;
    static uint8_t current_pwm_;
    static uint32_t last_time_;
};

template<typename Motor>
bool Mower<Motor>::is_enable_;

template<typename Motor>
uint8_t Mower<Motor>::pwm_;

template<typename Motor>
uint8_t Mower<Motor>::current_pwm_;

template<typename Motor>
uint32_t Mower<Motor>::last_time_;

#endif
