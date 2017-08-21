#ifndef MOTORS_H
#define MOTORS_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
}

#include "portable/atmega88pa/pio.h"

typedef PioD7 LeftMotor1Pin;
typedef PioB0 LeftMotor2Pin;
typedef PioB1 RightMotor1Pin;
typedef PioC5 RightMotor2Pin;

#define LEFT_PWM (OCR0A)
#define RIGHT_PWM (OCR0B)

class Motors
{
public:
    enum {
        UP    = -1,
        STOP  = 0,
        DOWN  = 1
    };

    enum { MAX_DUTY = 0xff };

public:
    static inline void init()
    {
        //ports
        LeftMotor1Pin::setOutput();
        LeftMotor2Pin::setOutput();
        RightMotor1Pin::setOutput();
        RightMotor2Pin::setOutput();

        PioD5::setOutput(); //OC0B
        PioD6::setOutput(); //OC0A

        //pwm
        TCCR0A |= (1<<COM0A1) | // OCR0A non-inverting mode
                  (1<<COM0B1) | // OCR0B non-inverting mode
                  (1<<WGM01) | (1<<WGM00); //Fast pwm

        TCCR0B |= (1<<CS00); //clk = F_CPU

        LEFT_PWM = 0;
        RIGHT_PWM = 0;

        set_dir(STOP, STOP);
    }

    static inline void set_duty(uint8_t left, uint8_t right)
    {
        LEFT_PWM = left;
        RIGHT_PWM = right;
    }

    static void set_dir(int8_t left, int8_t right)
    {
        //left
        LeftMotor1Pin::setLow();
        LeftMotor2Pin::setLow();
        if(left > 0)
            LeftMotor1Pin::setHigh();
        if(left < 0)
            LeftMotor2Pin::setHigh();

        //right
        RightMotor1Pin::setLow();
        RightMotor2Pin::setLow();
        if(right > 0)
            RightMotor1Pin::setHigh();
        if(right < 0)
            RightMotor2Pin::setHigh();
    }
};


#endif
