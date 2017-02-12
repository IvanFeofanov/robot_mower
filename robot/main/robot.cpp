#include "robot.h"

void Robot::init()
{
    //hardware
    Serial::init();
    Time::init();
    TwiMaster::init();
    MowerMotor::init();

    sei();

    //processes
    terminal_.init();
    button_.init();
    led_indicator_.init();
    mower_.init();
    drive_motors_.init();

    //state
    status_ = 0;

    //mower
    mower_msg_.speed = 255; //max
    mower_msg_.is_enable = false;

    //drive motors
    drive_motors_msg_.left_set_speed = 0;
    drive_motors_msg_.right_set_speed = 0;
    drive_motors_msg_.left_real_speed = 0;
    drive_motors_msg_.right_real_speed = 0;
    drive_motors_msg_.left_counter = 0;
    drive_motors_msg_.right_counter = 0;
}

void Robot::loop()
{
    terminal_.run(&status_, &mower_msg_, &drive_motors_msg_);

    button_.run(&status_);

    led_indicator_.run(&status_);

    mower_.run(&mower_msg_);

    drive_motors_.run(&drive_motors_msg_);
}
