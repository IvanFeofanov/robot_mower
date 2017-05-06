extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "debug.h"

//portable
#include "portable/atmega328/time.h"
#include "portable/atmega328/uart.h"
#include "portable/atmega328/twi_master.h"
#include "portable/atmega328/pio.h"
#include "portable/atmega328/mower_motor.h"
#include "portable/atmega328/adc_man.h"

//automats
#include "processes/one_button.h"
#include "processes/bumper.h"
#include "processes/perimeter_sensor.h"
#include "processes/one_led.h"
#include "processes/mower.h"
#include "processes/motors_controller_client.h"
#include "robot.h"


// hardware
enum {  MOTOR_CONTROLLER_TWI_ADDRESS = 2 };
typedef PioB5 LedPin;
typedef PioD6 ButtonPin;
typedef Adc0 AdcLeftPot;
typedef Adc1 AdcRightPot;
typedef Adc2 AdcPerimeterSensor;

// debug
typedef SerialDebug<Serial> Debug;

// automats
typedef OneButton<ButtonPin, true> Button;
typedef BumperSensor<AdcLeftPot, AdcRightPot> Bumper;
typedef PerimeterSensor<AdcPerimeterSensor> Perimeter;
typedef OneLed<LedPin> LedIndicator;
typedef Mower<MowerMotor> Mower_;
typedef MotorsControllerClient<TwiMaster, MOTOR_CONTROLLER_TWI_ADDRESS> DriveMotors;
typedef Robot<
            Button,
            Bumper,
            Perimeter,
            LedIndicator,
            Mower_,
            DriveMotors,
            Debug> Robot_;


static inline void init()
{
    // hardware
    Time::init();
    Adc::init();
    MowerMotor::init();
    TwiMaster::init();
    Serial::init();

    sei(); //enable interrupts

    Button::init();
    Bumper::init();
    Perimeter::init();
    LedIndicator::init();
    Mower_::init();
    DriveMotors::init();

    Robot_::init();
}

static inline void loop()
{
    Button::update();
    Bumper::update();
    Perimeter::update();
    LedIndicator::update();
    Mower_::update();
    DriveMotors::update();

    Robot_::update();
}

int main()
{
    init();

    while(true){
        loop();
    }

    return 0;
}
