extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "portable/atmega328/uart.h"
#include "portable/atmega328/motors.h"
#include "portable/atmega328/odometers.h"
#include "portable/atmega328/time.h"

#include "processes/terminal.h"
#include "processes/motors_controller.h"
#include "processes/mediator.h"

//process
Terminal<Serial, Time> terminal;
MotorsController<Motors, Odometers, Time> motors_controller;

static inline void hardwareInit()
{
    Serial::init();
    Motors::init();
    Odometers::init();
    Time::init();

    DDRB |= (1<<5); //led

    sei();
}

static inline void processesInit()
{
    terminal.init();
    motors_controller.init();
    Mediator::init();
}

static inline void loop()
{
    terminal.run();
    motors_controller.run();
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
