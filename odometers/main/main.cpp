extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "portable/atmega328/uart.h"

char buffer []  = "hello\n";

void setup()
{
    DDRB = (1<<5);
    Serial::begin();
    sei();

    _delay_ms(500);
    Serial::write(buffer, strlen(buffer));
}

void loop()
{
    int length = Serial::available();

    if(length > 0){
        Serial::read(buffer, length);
        while(!Serial::isWriten());
        Serial::write(buffer, length);
    }
}

int main()
{
    setup();
    while(true)
        loop();

    return 0;
}
