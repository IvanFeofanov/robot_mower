#ifndef CHASSIS_H
#define CHASSIS_H

#include <stdlib.h>
#include <avr/io.h>

#include "process.h"
#include "messages.h"

#include "../utils/pid.h"

#include "types/diff_control.h"

template<
    typename Drive,
    typename Odometers,
    uint16_t COMMAND_CHANGED
    >
class Chassis: public Process
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
            float left_rps_feedback = (float)Odometers::getLeft();
            float output = regulator_.update(left_rps_input_, left_rps_feedback);
            Drive::setPwm((uint8_t)(output));

            last_time_ = Time::now();
        }

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

            Drive::setDirection((uint8_t)command->left_speed);
        }
    }

    void setPidCoeff()
    {
    //
    }

private:
    Pid regulator_;
    uint32_t last_time_;

    float left_rps_input_;
    float right_rps_input_;


};

#endif
