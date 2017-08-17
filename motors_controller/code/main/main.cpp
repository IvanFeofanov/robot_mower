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
#include "processes/spi_face.h"
// debug

// hardware
typedef PioC2 Led1;
typedef PioC3 Led2;

// automats
typedef MotorsController<Motors, Odometers> MotorsCtrl;
typedef SpiFace<SpiSlave, Odometers,  MotorsCtrl> SpiInterface;

static inline void init()
{
    Led1::setOutput();
    Led2::setOutput();

    Motors::init();
    Odometers::init();
    SpiSlave::init();

    MotorsCtrl::init();
    SpiInterface::init();

    sei();
}

static inline void loop()
{
    MotorsCtrl::update();
    SpiInterface::update();
}

int main()
{
    init();

    while(true){
        loop();
    }

    return 0;
}
