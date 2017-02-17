#include "robot.h"

void Robot::init()
{
    //hardware
    Serial::init();
    Time::init();
    TwiMaster::init();
    MowerMotor::init();
    Adc::init();

    sei();

    //processes
    terminal_.init();
    button_.init();
    bumper_.init();
    perimeter_sensor_.init();
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

    //bumper
    bumper_msg_.left = 0;
    bumper_msg_.left_min = 0;
    bumper_msg_.left_max = 1023;

    bumper_msg_.right = 0;
    bumper_msg_.right_min = 0;
    bumper_msg_.right_max = 1023;

    bumper_msg_.is_calibration = false;

    //perimeter sensor
    perimeter_sensor_msg_.signal_offset = 512;

    perimeter_sensor_msg_.lps = 0;

    perimeter_sensor_msg_.magnitude = 0;
    perimeter_sensor_msg_.smooth_magnitude = 0;
    perimeter_sensor_msg_.is_inside = false;

    perimeter_sensor_msg_.is_offset_calibration_mode = false;
}

void Robot::loop()
{
    terminal_.run(&status_, &mower_msg_, &drive_motors_msg_, &bumper_msg_,
            &perimeter_sensor_msg_);

    button_.run(&status_);

    bumper_.run(&bumper_msg_);

    perimeter_sensor_.run(&perimeter_sensor_msg_);

    led_indicator_.run(&status_);

    mower_.run(&mower_msg_);

    drive_motors_.run(&drive_motors_msg_);
}
