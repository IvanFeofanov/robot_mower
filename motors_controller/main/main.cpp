extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#ifdef __AVR_ATmega328P__
#   include "portable/atmega328/uart.h"
#   include "portable/atmega328/motors.h"
#   include "portable/atmega328/odometers.h"
#   include "portable/atmega328/twi_slave.h"
#endif

#ifdef __AVR_ATmega88P__
#   include "portable/atmega88pa/uart.h"
#   include "portable/atmega88pa/motors.h"
#   include "portable/atmega88pa/odometers.h"
#   include "portable/atmega88pa/twi_slave.h"
#endif

#include "../config.h"

#include "processes/terminal.h"
#include "processes/motors_controller.h"
#include "processes/twi_interface.h"
#include "processes/mediator.h"


//process
Terminal<Serial> terminal;
MotorsController<Motors, Odometers> motors_controller;

static inline void hardwareInit()
{
    Serial::init();
    Motors::init();
    Odometers::init();
    Twi::init(TWI_DEVICE_ADDRESS);

    // DDRB |= (1<<5); //led
    DDRC |= (1<<2) | (1<<3);

    sei();
}

static inline void processesInit()
{
    terminal.init();
    motors_controller.init();
    TwiInterface<Twi>::init();
    Mediator::init();
}

static inline void loop()
{
    terminal.run();
    motors_controller.run();
}

// uint8_t get_buffer[32];
int main()
{
    hardwareInit();
    processesInit();

    // _delay_ms(1000);

    // PORTB |= (1<<5);
    // char debug[32];
    // for(int i = 0; i < Twi::work_log_index_; i++){
    //     sprintf(debug, "%x ", Twi::work_log_[i]);
    //     Serial::write(debug, strlen(debug));
    //     while(!Serial::isWriten());
    // }
    // Twi::work_log_index_ = 0;
    //

    while(true){
        loop();
    }

    return 0;
}
