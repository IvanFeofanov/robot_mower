#ifndef MOTORS_CONTROLLER_H
#define MOTORS_CONTROLLER_H

#include "mediator.h"

template<
    typename Motors,
    typename Odometers,
    typename Time
    >
class MotorsController
{
public:
    enum {  LEFT = Mediator::LEFT_MOTOR,
            RIGHT = Mediator::RIGHT_MOTOR };

    void init()
    {
        last_time_ = 0;
    }

    void run()
    {
        //update speed
        if(Time::now() - last_time_ >= 100)
        {
            //left
            Mediator::real_rps[LEFT] = Odometers::getLeftRps();
            Mediator::real_rps[RIGHT] = Odometers::getRightRps();
            uint16_t left_pwm = Mediator::pid[LEFT].update(
                    Mediator::set_rps[LEFT] < 0 ?
                    -Mediator::set_rps[LEFT] :
                    Mediator::set_rps[LEFT],
                    Mediator::real_rps[LEFT]);

            uint16_t right_pwm = Mediator::pid[RIGHT].update(
                    Mediator::set_rps[RIGHT] < 0 ?
                    -Mediator::set_rps[RIGHT] :
                    Mediator::set_rps[RIGHT],
                    Mediator::real_rps[RIGHT]);


            Motors::setPwm(left_pwm, right_pwm);
            Motors::setDirection(Mediator::set_rps[LEFT],
                                 Mediator::set_rps[RIGHT]);

            last_time_ = Time::now();
        }
    }

private:
    uint32_t last_time_;

};

#endif
