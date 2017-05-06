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
        DriveMotors::attachSpeedDataChange(&newSpeed);
        DriveMotors::attachOdometerDataChange(&newOdometer);
    }

    static void update()
    {
        DriveMotors::setSpeed(100, 50);
    }

    static void newSpeed(int16_t left, int16_t right)
    {
        Debug::print("speed: l = ");
        Debug::print(left);
        Debug::print(" r = ");
        Debug::print(right);
        Debug::print(" ");
    }

    static void newOdometer(uint32_t left, uint32_t right)
    {
        Debug::print("odometers: l = ");
        Debug::print(left);
        Debug::print(" r = ");
        Debug::print(right);
        Debug::println();

    }

private:

};

#endif
