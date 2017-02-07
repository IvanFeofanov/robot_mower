extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "portable/atmega328/uart.h"
#include "portable/atmega328/time.h"
#include "portable/atmega328/twi_master.h"

#include "processes/terminal.h"

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

//process
Terminal<Serial, Time> terminal;

static inline void hardwareInit()
{
    Serial::init();
    Time::init();
    TwiMaster::init();

    DDRB |= (1<<5); //led

    sei();
}

static inline void processesInit()
{

}

static inline void loop()
{
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
