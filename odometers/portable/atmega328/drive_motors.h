#ifndef DRIVE_MOTORS_H
#define DRIVE_MOTORS_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
}

#include "disable_interrupt.h"

class DriveMotors
{
public:
    static void init()
    {
        //ports
        DDRD |= (1<<7);
        DDRB |= (1<<0) | (1<<3);

        //pwm
        TCCR2A |= (1<<COM2A1) | (1<<WGM21) | (1<<WGM20); //Fast pwm
        TCCR2B |= (1<<CS20); //clk = F_CPU
        OCR2A = 0;

        //external interrupt
        EICRA |= (1<<ISC00);
        EIMSK |= (1<<INT0);

        setDirection(-1);

        counter_ = 0;
    }

    static uint32_t getCounter()
    {
        DisableInterrupt di;

        uint32_t tmp = counter_;
        counter_ = 0;

        return tmp;
    }

    static void resetCounter()
    {
        counter_ = 0;
    }

    static void setPwm(uint8_t value)
    {
        OCR2A = value;
    }

    static void setDirection(int8_t sig)
    {
        //TODO вынести в порты в параметры шаблона
        if(sig > 0){
            PORTD &= ~(1<<7);
            PORTB |= (1<<0);
        }else if(sig < 0){
            PORTD |= (1<<7);
            PORTB &= ~(1<<0);
        }else{
            PORTD &= ~(1<<7);
            PORTB &= ~(1<<0);
        }
    }

public:
    volatile static void odometerInterrupt()
    {
        counter_++;
    }

private:
    volatile static uint32_t counter_;
};


#endif
