extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "portable/atmega328/uart.h"
#include "portable/atmega328/motors.h"
#include "portable/atmega328/odometers.h"
#include "portable/atmega328/time.h"
#include "portable/atmega328/twi.h"

#include "processes/terminal.h"
#include "processes/motors_controller.h"
#include "processes/mediator.h"

//process
Terminal<Serial, Time> terminal;
MotorsController<Motors, Odometers, Time> motors_controller;

static inline void hardwareInit()
{
    Serial::init();
    Motors::init();
    Odometers::init();
    Time::init();
    I2c::init();

    DDRB |= (1<<5); //led

    sei();
}

static inline void processesInit()
{
    terminal.init();
    motors_controller.init();
    Mediator::init();
}

static inline void loop()
{
    terminal.run();
    motors_controller.run();
}

char* buffer;

int main()
{
    hardwareInit();
    processesInit();

    buffer = "hello";

    I2c::write(4, (uint8_t*)buffer, 5);
    _delay_ms(100);
    char debag_buffer[32];
    for(int i = 0; i < I2c::work_log_index_; i++){
        sprintf(debag_buffer, "%x ", I2c::work_log_[i]);
        Serial::write(debag_buffer, strlen(debag_buffer));
        while(!Serial::isWriten());
        // PORTB |= (1<<5);
    }

    while(true){
        loop();
    }

    return 0;
}
