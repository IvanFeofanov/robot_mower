extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

// portable
#include "portable/atmega88pa/motors.h"
#include "portable/atmega88pa/odometers.h"
#include "portable/atmega88pa/pio.h"
#include "portable/atmega88pa/spi.h"
#include "portable/atmega88pa/twi_slave.h"

// automats
#include "processes/motors_controller.h"
#include "processes/twi_face.h" 

// debug

// hardware
typedef PioC2 Led1;
typedef PioC3 Led2;

// automats
typedef MotorsController<Motors, Odometers> MotorsCtrl;
typedef TwiFace<Twi, Odometers,  MotorsCtrl> TwiInterface;

static inline void init()
{
    Led1::setOutput();
    Led2::setOutput();

    Motors::init();
    Odometers::init();

    MotorsCtrl::init();
    TwiInterface::init();

    sei();
}

static inline void loop()
{
    MotorsCtrl::update();
    TwiInterface::update();
}

int main()
{
    init();

    while(true){
        loop();
    }

    return 0;
}
