extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

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

#define MOTOR_CONTROLLER_TWI_ADDRESS (2)

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}


//hardware
typedef PioB5 LedPin;
typedef PioD6 ButtonPin;

//process
Terminal<Serial, Time> terminal;
LedIndiactor<LedPin, Time> led_indicator;
Button<ButtonPin, Time> button;
Mower<MowerMotor, Time> mower;
DriveMotors<TwiMaster, MOTOR_CONTROLLER_TWI_ADDRESS> drive_motors;


static inline void hardwareInit()
{
    Serial::init();
    Time::init();
    TwiMaster::init();
    MowerMotor::init();

    sei();
}

static inline void processesInit()
{
    terminal.init();
    button.init();
    led_indicator.init();
    mower.init();
    drive_motors.init();
}

static inline void loop()
{
    static uint8_t status = 0;

    static uint8_t mower_speed = 255;

    static int16_t left_set_speed = 60;
    static int16_t right_set_speed = 60;
    static int16_t left_real_speed = 0;
    static int16_t right_real_speed = 0;
    static uint32_t left_counter = 0;
    static uint32_t right_counter = 0;

    terminal.run();
    button.run(&status);
    led_indicator.run(&status);
    mower.run(mower_speed);
    drive_motors.run(left_set_speed, right_set_speed,
                     &left_real_speed, &right_real_speed,
                     &right_counter, &left_counter);
}

int main()
{
    hardwareInit();
    processesInit();

    while(true){
        loop();
    }

    return 0;
}
