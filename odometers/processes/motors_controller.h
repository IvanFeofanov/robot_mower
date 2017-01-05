#ifndef MOTORS_CONTROLLER_H
#define MOTORS_CONTROLLER_H

#include <stdlib.h>
#include <avr/io.h>

#include "process.h"
#include "messages.h"

#include "../utils/pid.h"

#include "types/diff_control.h"

template<
    typename Motors,
    typename Odometers,
    typename Time,
    uint16_t COMMAND_CHANGED,
    uint16_t PID_COEFF_CHANGED,
    uint16_t SPEED_OBTAINED
    >
class MotorsController: public Process
{
public:
    void init()
    {
        last_time_ = 0;

        left_rps_input_     = 0;
        right_rps_input_    = 0;
    }

    void run()
    {
        //update speed
        if(Time::now() - last_time_ >= 100)
        {
            float output = 0;

            //left
            left_rps_feedback_ = (float)Odometers::getLeftRps();
            output = left_regulator_.update(left_rps_input_, left_rps_feedback_);
            Motors::setLeftPwm((uint8_t)(output));

            // right_rps_feedback_ = (float)Odometers::getRightCounter();
            // output = right_regulator.update(right_rps_input_, right_rps_feedback_);
            // Motors::setRightPwm((uint8_t)(output));

            last_time_ = Time::now();
        }

        //send speed
        Messages::send(SPEED_OBTAINED, &left_rps_feedback_);

    }

    void handleMessages()
    {
        setCommand();
        setPidCoeff();
    }

    void setCommand()
    {
        DiffControl* command;
        if(Messages::get(COMMAND_CHANGED, (void**)(&command))){
            //set speed
            left_rps_input_ = abs(command->left_speed);
            right_rps_input_ = abs(command->right_speed);

            Motors::setLeftDirection((uint8_t)command->left_speed);
            Motors::setRightDirection((uint8_t)command->right_speed);
        }
    }

    void setPidCoeff()
    {
        Pid* pid;
        if(Messages::get(PID_COEFF_CHANGED, (void**)(&pid))){
            left_regulator_ = *pid;
        }
    }


private:
    Pid left_regulator_;
    Pid right_regulator_;

    uint32_t last_time_;

    float left_rps_input_;
    float left_rps_feedback_;
    float right_rps_input_;
    float right_rps_feedback_;

};

#endif
