#ifndef ROBOT_H
#define ROBOT_H

#include "portable/atmega328/uart.h"
#include "portable/atmega328/time.h"
#include "portable/atmega328/twi_master.h"
#include "portable/atmega328/pio.h"
#include "portable/atmega328/mower_motor.h"
#include "portable/atmega328/adc_man.h"

#include "processes/terminal.h"
#include "processes/button.h"
#include "processes/bumper.h"
#include "processes/perimeter_sensor.h"
#include "processes/algorithm.h"
#include "processes/led_indicator.h"
#include "processes/mower.h"
#include "processes/drive_motors.h"

#include "types.h"

class Robot
{
public:
    void init();
    void loop();

private:
    enum {  MOTOR_CONTROLLER_TWI_ADDRESS = 2 };

    //hardware
    typedef PioB5 LedPin;
    typedef PioD6 ButtonPin;
    typedef Adc0 AdcLeftPot;
    typedef Adc1 AdcRightPot;
    typedef Adc2 AdcPerimeterSensor;

    //process
    Terminal<Serial, Time> terminal_;
    Button<ButtonPin, Time> button_;
    Bumper<Time, AdcLeftPot, AdcRightPot> bumper_;
    PerimeterSensor<Time, AdcPerimeterSensor> perimeter_sensor_;
    Algorithm<Time> algorithm_;
    LedIndiactor<LedPin, Time> led_indicator_;
    Mower<MowerMotor, Time> mower_;
    DriveMotors<TwiMaster, MOTOR_CONTROLLER_TWI_ADDRESS> drive_motors_;

    //msgs
    uint8_t status_;
    MowerMsg mower_msg_;
    DriveMotorsMsg drive_motors_msg_;
    BumperMsg bumper_msg_;
    PerimeterSensorMsg perimeter_sensor_msg_;
};

#endif
