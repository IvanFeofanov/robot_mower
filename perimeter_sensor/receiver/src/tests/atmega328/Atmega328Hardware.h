#ifndef ATMEGA328_HARDWARE_H
#define ATMEGA328_HARDWARE_H

extern "C"
{
#include <avr/interrupt.h>
#include "avr_time.h"
#include "avr_uart.h"
}

class Atmega328Hardware
{
public:
    Atmega328Hardware() {}

    void init()
    {
        avr_time_init();
        avr_uart_init();
        sei();
    }

    //Read a byte of data from ROS connection
    //If no data - returns - 1
    int16_t read()
    {
        return avr_uart_receive_byte();
    }

    //Send a byte of data to ROS connection
    void write(char* data, uint32_t length)
    {
        for(uint32_t i = 0; i < length; i++){
            avr_uart_send_byte(data[i]);
        }
    }

    //Return miliseconds since start of program
    uint32_t time()
    {
        return avr_time_now();
    }

};

#endif
