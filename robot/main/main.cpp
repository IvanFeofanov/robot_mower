extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "robot.h"

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

Robot robot;

static inline void init()
{
    robot.init();
}

static inline void loop()
{
    robot.loop();
}

int main()
{
    init();

    while(true){
        loop();
    }

    return 0;
}
