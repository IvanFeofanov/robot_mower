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
#include "portable/atmega328/time.h"

#include "fsm/terminal.h"
#include "fsm/chassis.h"

//messages
enum{
    UP
};

//process
Terminal<Serial, Time, DriveMotors> terminal;
Chassis<DriveMotors, UP> chassis;

inline void hardwareInit()
{
    Serial::init();
    DriveMotors::init();
    Time::init();

    DDRB |= (1<<5); //led

    sei();
}

inline void fsmInit()
{
    terminal.init();
    chassis.init();
}

void loop()
{
    terminal.run();
    chassis.run();
}

int main()
{
    hardwareInit();
    fsmInit();

    MsgHandler::init();

    while(true){
        loop();
        MsgHandler::run();
    }

    return 0;
}
