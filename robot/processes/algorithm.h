#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "pid.h"
#include "types.h"

template<
    typename Time
    >
class Algorithm
{
public:
    enum { WORK_SPEED = 70 };
    enum { MAX_SPEED = 110 };
    void init()
    {
        // pid_ = Pid(100.0, -100.0, 10.0, 10.0, 0.0);
    }

    void run( PerimeterSensorMsg* perimeter_sensor,
              BumperMsg* bumper,
              MowerMsg* mower,
              DriveMotorsMsg* drive_motors)
    {
        if(perimeter_sensor->is_new_data){
            perimeter_sensor->is_new_data = false;
            int8_t perimeter_value = 0;
            if(perimeter_sensor->magnitude < -0){
                perimeter_value = -1; //outside
                PORTB &= ~(1<<5);
            }else if(perimeter_sensor->magnitude > 0){
                perimeter_value = 1; //inside
                PORTB |= (1<<5);
            }else{
                perimeter_value = 0; //under wire
            }

            int16_t left_speed = 0;
            int16_t right_speed = 0;

            int16_t correction  = (int16_t)pid_.update(0.0, (float)perimeter_value);

            left_speed = WORK_SPEED + correction;
            if(left_speed > MAX_SPEED){
                left_speed = MAX_SPEED;
            }
            if(left_speed < -MAX_SPEED){
                left_speed = -MAX_SPEED;
            }

            right_speed = WORK_SPEED - correction;
            if(right_speed > MAX_SPEED){
                right_speed = MAX_SPEED;
            }
            if(right_speed < -MAX_SPEED){
                right_speed = -MAX_SPEED;
            }

            drive_motors->setSpeed(left_speed, right_speed);
            // drive_motors->setSpeed(WORK_SPEED, WORK_SPEED);
        }
    }



private:
   Pid pid_;
};

#endif
