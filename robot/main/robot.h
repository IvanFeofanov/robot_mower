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
    typename DriveMotors,
    typename Debug
    >
class Robot
{
public:
    static void init()
    {
        Debug::print("Robot init\n");
        DriveMotors::set_left_speed(0);
        DriveMotors::set_right_speed(0);
    }

    static void update()
    {
        DriveMotors::set_left_speed(50);
        DriveMotors::set_right_speed(50);

        Debug::print("speed: l = ");
        Debug::print(DriveMotors::real_left_speed());
        Debug::print(" r = ");
        Debug::print(DriveMotors::real_right_speed());
        Debug::println();

        Debug::print("odometers: l = ");
        Debug::print(DriveMotors::left_counter());
        Debug::print(" r = ");
        Debug::print(DriveMotors::right_counter());
        Debug::println();
    }

private:

};

#endif
