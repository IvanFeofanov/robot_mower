#ifndef MOWER_H
#define MOWER_H

#include "map.h"

template<
    typename Motor,
    typename Time
    >
class Mower
{
public:
    enum {
        MIN_SPEED = 0,
        MAX_SPEED = 255
    };

    enum {
        TIME_STEP = 10, //ms
        SPEED_STEP = 1
    };

    void init()
    {
        //drive motor
        Motor::setPwm(Motor::MIN_PWM);
    }

    void run(const uint8_t speed)
    {
        static uint32_t last_time = 0;
        static uint8_t current_speed = MIN_SPEED;

        if(speed == MIN_SPEED){ //stop
            Motor::setPwm(Motor::MIN_PWM);
            current_speed = MIN_SPEED;
            return;
        }

        if(Time::now() - last_time > TIME_STEP){
            uint8_t pwm = map(current_speed, MIN_SPEED, MAX_SPEED,
                    Motor::MIN_PWM, Motor::MAX_PWM);

            Motor::setPwm(pwm);

            if(current_speed < speed){
                current_speed += SPEED_STEP;
            }

            if(current_speed > speed){
                current_speed -= SPEED_STEP;
            }

            last_time = Time::now();
        }
    }
};

#endif
