#ifndef ROBOT_H
#define ROBOT_H

#include "pid.h"
#include "stack.h"

template<
    typename Button,
    typename Bumper,
    typename PerimeterSensor,
    typename LedIndicator,
    typename Mower,
    typename DriveMotors
    >
class Robot
{
public:
    static void init()
    {
    }

    static void update()
    {
    }

private:
    // //algorith
    // enum { WORK_SPEED = 100 };
    // enum { MAX_SPEED = 110 };
    //
    // Pid pid_;
    //
    // Stack<int, 10> state_;
    //
    // enum{
    //     state_wait,
    //     state_stop,
    //     state_perimeter,
    //     state_avoid_stuff,
    //     state_fill
    // };
    //
    // void stopMotors();
    // void perimeterFlow();
    // bool isStuffOnWay();
    // void avoidStuff();
};

#endif
