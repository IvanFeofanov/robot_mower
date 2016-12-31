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

        setLeftDirection(STOP);
        setRightDirection(STOP);
    }

    static void setLeftPwm(uint8_t value)
    {
        OCR2A = value;
    }

    static void setRightPwm(uint8_t value)
    {
        //TODO
    }

    static uint8_t getLeftPwm()
    {
        return OCR2A;
    }

    static uint8_t getRightPwm()
    {
        return 0; //TODO
    }

    static void setLeftDirection(int8_t sig)
    {
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

    static void setRightDirection(int8_t sig)
    {
        //TODO
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
