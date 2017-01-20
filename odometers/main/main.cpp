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
#include "portable/atmega328/twi_slave.h"

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
    Twi::init(2);

    DDRB |= (1<<5); //led

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
        // PORTB ^= (1<<5);
        // _delay_ms(50);
    }

    return 0;
}
