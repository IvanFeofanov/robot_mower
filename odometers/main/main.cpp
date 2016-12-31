extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "process.h"
#include "messages.h"

#include "portable/atmega328/uart.h"
#include "portable/atmega328/motors.h"
#include "portable/atmega328/odometers.h"
#include "portable/atmega328/time.h"

#include "processes/terminal.h"
#include "processes/motors_controller.h"

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

//messages
enum{
    COMMAND_CHANGED,
    PID_COEFF_CHANGED,
    SPEED_OBTAINED
};

//process
Terminal<Serial, Time,
        COMMAND_CHANGED, PID_COEFF_CHANGED, SPEED_OBTAINED> terminal;
MotorsController<Motors, Odometers, Time,
        COMMAND_CHANGED, PID_COEFF_CHANGED, SPEED_OBTAINED> motors_controller;

Process* processes []  = {
    &terminal,
    &motors_controller
};

inline void hardwareInit()
{
    Serial::init();
    Motors::init();
    Odometers::init();
    Time::init();

    DDRB |= (1<<5); //led

    sei();
}

inline void processesInit()
{
    for(uint8_t i = 0; i < sizeof(processes) / sizeof(void*); i++)
        processes[i]->init();
}

void loop()
{
    for(uint8_t i = 0; i < sizeof(processes) / sizeof(void *); i++){
        processes[i]->handleMessages();
        processes[i]->run();
    }
}

int main()
{
    hardwareInit();
    processesInit();

    Messages::init();

    while(true){
        loop();
        Messages::run();
    }

    return 0;
}
