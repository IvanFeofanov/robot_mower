extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "portable/atmega328/uart.h"
#include "portable/atmega328/drive_motors.h"
#include "fsm/terminal.h"

Terminal<Serial, DriveMotors> terminal;

void setup()
{
    Serial::init();
    DriveMotors::init();
    DDRB |= (1<<5);
    sei();
}

void loop()
{
    terminal.exec();
}

int main()
{
    setup();
    while(true)
        loop();

    return 0;
}
