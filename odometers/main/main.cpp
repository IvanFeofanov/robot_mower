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
#include "portable/atmega328/time.h"

#include "fsm/terminal.h"

Terminal<Serial, DriveMotors> terminal;

// static uint32_t last_time;

void setup()
{
    Serial::init();
    DriveMotors::init();
    Time::init();

    DDRB |= (1<<5); //led
    // last_time = 0;

    sei();
}

void loop()
{
    terminal.exec();
    // if(Time::now() - last_time >= 1000)
    // {
    //     PORTB ^= (1<<5);
    //     last_time = Time::now();
    // }
}

int main()
{
    setup();
    while(true)
        loop();

    return 0;
}
