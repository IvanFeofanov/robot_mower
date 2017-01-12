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
    static inline void init()
    {
        //ports
        DDRD |= (1<<6) | (1<<7);
        DDRB |= (1<<3);

        //pwm
        TCCR2A |= (1<<COM2A1) | (1<<WGM21) | (1<<WGM20); //Fast pwm
        TCCR2B |= (1<<CS20); //clk = F_CPU

        OCR2A = 0;
        OCR2A = 0;

        setDirection(STOP, STOP);
    }

    static inline void setPwm(uint8_t left, uint8_t right)
    {
        OCR2A = left;
        //TODO = right;
        OCR2A = left;
    }

    static void setDirection(int8_t left, uint8_t right)
    {
        PORTD &= ~(1<<7);
        PORTD &= ~(1<<6);
        if(left > 0)
            PORTD |= (1<<6);
        if(left < 0)
            PORTD |= (1<<7);

        //TODO right
        PORTD &= ~(1<<7);
        PORTD &= ~(1<<6);
        if(left > 0)
            PORTD |= (1<<6);
        if(left < 0)
            PORTD |= (1<<7);
    }

public:
    enum {
        RIGHT = -1,
        STOP,
        LEFT
    };

    enum { MAX_PWM = 0xff };
};


#endif
