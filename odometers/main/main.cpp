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
#include "portable/atmega328/drive_motors.h"
#include "portable/atmega328/odometers.h"
#include "portable/atmega328/time.h"

#include "processes/terminal.h"
#include "processes/chassis.h"

//messages
enum{
    COMMAND_CHANGED
};

//process
Terminal<Serial, Time, COMMAND_CHANGED> terminal;
Chassis<DriveMotors, Odometers, COMMAND_CHANGED> chassis;

// Process* processes []  = {
    // &terminal,
    // &chassis
// };

inline void hardwareInit()
{
    Serial::init();
    DriveMotors::init();
    Odometers::init();
    Time::init();

    DDRB |= (1<<5); //led

    sei();
}

inline void processesInit()
{
    terminal.init();
    chassis.init();
    // for(int i = 0; i < sizeof(processes) / sizeof(void*); i++)
        // processes[i]->init();
}

void loop()
{
    terminal.handleMessages();
    terminal.run();
    chassis.handleMessages();
    chassis.run();
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
