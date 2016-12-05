extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "portable/atmega328/uart.h"

void setup()
{
    Uart9600::init();
}

void loop()
{
}


int main()
{
    setup();
    while(true)
        loop();

    return 0;
}
