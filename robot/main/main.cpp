extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// #include <stdio.h>
// #include <string.h>
}

//portable
#include "portable/atmega328/uart.h"
#include "portable/atmega328/time.h"
#include "portable/atmega328/twi_master.h"
#include "portable/atmega328/pio.h"
#include "portable/atmega328/mower_motor.h"
#include "portable/atmega328/adc_man.h"

//automats
// #include "processes/terminal.h"
#include "processes/one_button.h"
#include "processes/bumper.h"
#include "processes/perimeter_sensor.h"
#include "processes/one_led.h"
#include "processes/mower.h"
#include "processes/drive_motors.h"
#include "robot.h"

//hardware
enum {  MOTOR_CONTROLLER_TWI_ADDRESS = 2 };
typedef PioB5 LedPin;
typedef PioD6 ButtonPin;
typedef Adc0 AdcLeftPot;
typedef Adc1 AdcRightPot;
typedef Adc2 AdcPerimeterSensor;

// sub automat
// Terminal<Serial, Time> terminal_;
typedef OneButton<ButtonPin, true> Button;
typedef Bumper<AdcLeftPot, AdcRightPot> Bumper_;
typedef PerimeterSensor<AdcPerimeterSensor> Perimeter;
typedef OneLed<LedPin> LedIndicator;
typedef Mower<MowerMotor> Mower_;
// DriveMotors<TwiMaster, MOTOR_CONTROLLER_TWI_ADDRESS> drive_motors_;


// void function()
// {
//     LedPin::setHigh();
//     static bool flag_ = false;
//     if(!flag_){
//         LedIndicator::blink();
//         flag_ = true;
//     }else{
//         LedIndicator::setLow();
//         flag_ = false;
//     }
// }
//

static inline void init()
{
    // hardware
    Adc::init();
    Time::init();
    MowerMotor::init();
    sei();

    Button::init();
    // Button::attachClick(&function);

    Bumper_::init();
    Perimeter::init();
    LedIndicator::init();
    // LedIndicator::blink();

    Mower_::init();
    // Mower_::setEnable(true);
    // Mower_::setSpeed(50);
}

static inline void loop()
{
    Button::update();
    Bumper_::update();
    Perimeter::update();
    LedIndicator::update();
    Mower_::update();
}

int main()
{
    init();

    while(true){
        loop();
    }

    return 0;
}
