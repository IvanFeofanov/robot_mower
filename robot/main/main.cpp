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
#include "portable/atmega328/pio.h"

#include "processes/terminal.h"
#include "processes/led_indicator.h"
#include "processes/button.h"

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

//process
Terminal<Serial, Time> terminal;

typedef PioB5 LedPin;
LedIndiactor<LedPin, Time> led_indicator;

typedef PioD6 ButtonPin;
Button<ButtonPin, Time> button;

static inline void hardwareInit()
{
    Serial::init();
    Time::init();
    TwiMaster::init();

    sei();
}

static inline void processesInit()
{
    terminal.init();
    led_indicator.init();
    button.init();
}

static inline void loop()
{
    static uint8_t status = 0;

    terminal.run();
    button.run(&status);
    led_indicator.run(&status);
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
