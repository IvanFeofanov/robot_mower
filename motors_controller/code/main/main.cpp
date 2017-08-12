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

// automats
#include "processes/motors_controller.h"

// debug

// hardware
typedef PioC2 Led1;
typedef PioC3 Led2;

// automats
typedef MotorsController<Motors, Odometers> MotorsCtrl;

static inline void init()
{
    Motors::init();
    Odometers::init();

    Led1::setOutput();
    Led2::setOutput();

    SpiSlave::init();

    sei();

    MotorsCtrl::init();
}

static inline void loop()
{
    // motors_controller.update();
}

int main()
{
    init();

    while(true){
        loop();
    }

    return 0;
}
