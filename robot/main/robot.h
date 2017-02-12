#ifndef ROBOT_H
#define ROBOT_H

#define MOTOR_CONTROLLER_TWI_ADDRESS (2)

#include "portable/atmega328/uart.h"
#include "portable/atmega328/time.h"
#include "portable/atmega328/twi_master.h"
#include "portable/atmega328/pio.h"
#include "portable/atmega328/mower_motor.h"

#include "processes/terminal.h"
#include "processes/button.h"
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
    //hardware
    typedef PioB5 LedPin;
    typedef PioD6 ButtonPin;

    //process
    Terminal<Serial, Time> terminal_;
    LedIndiactor<LedPin, Time> led_indicator_;
    Button<ButtonPin, Time> button_;
    Mower<MowerMotor, Time> mower_;
    DriveMotors<TwiMaster, MOTOR_CONTROLLER_TWI_ADDRESS> drive_motors_;

    //msgs
    uint8_t status_;
    MowerMsg mower_msg_;
    DriveMotorsMsg drive_motors_msg_;

};

#endif
