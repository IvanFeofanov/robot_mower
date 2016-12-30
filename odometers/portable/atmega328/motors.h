#ifndef MOTORS_H
#define MOTORS_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
}

class Motors
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

        setDirection(1);
    }

    static void setPwm(uint8_t value)
    {
        OCR2A = value;
    }

    static uint8_t getPwm()
    {
        return OCR2A;
    }

    static void setDirection(int8_t sig)
    {
        //TODO вынести порты в параметры шаблона
        if(sig > 0){
            //left
            PORTD &= ~(1<<7);
            PORTB |= (1<<0);
        }else if(sig < 0){
            //right
            PORTD |= (1<<7);
            PORTB &= ~(1<<0);
        }else{
            //stop
            PORTD &= ~(1<<7);
            PORTB &= ~(1<<0);
        }
    }

public:
    enum {
        RIGHT = -1,
        STOP,
        LEFT
    };
};


#endif
