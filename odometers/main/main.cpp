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
#include "portable/atmega328/twi_slave.h"

#include "processes/terminal.h"
#include "processes/motors_controller.h"
#include "processes/mediator.h"

//process
Terminal<Serial, Time> terminal;
MotorsController<Motors, Odometers, Time> motors_controller;

char* buffer = "hello ";

void receiveEvent(uint8_t* data, uint8_t length){
}

void requestEvent(uint8_t* request, uint8_t length){
    buffer[0] = 1 + request[0];
    Twi::write((uint8_t*)buffer, 1);
}

static inline void hardwareInit()
{
    Serial::init();
    Motors::init();
    Odometers::init();
    Time::init();
    Twi::init(2);
    Twi::onRequest(&requestEvent);

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

uint8_t get_buffer[32];
int main()
{
    hardwareInit();
    processesInit();

    // _delay_ms(1000);
    //
    // // PORTB |= (1<<5);
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
