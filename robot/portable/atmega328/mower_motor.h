#ifndef MOWER_MOTOR_H
#define MOWER_MOTOR_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
}

class MowerMotor
{
public:
    enum {
        MIN_PWM = 0,
        MAX_PWM = 255
     };

public:
    static inline void init()
    {
        // Timer/Counter2 with pwm
        TCCR2A  = (1<<COM2B1) // OC2A non-inverting mode
                | (1<<WGM21)  // fast pwm
                | (1<<WGM20);
        TCCR2B = (1<<CS20);   // clk/1

        //port
        DDRD |= (1<<3);

    }

    static inline void setPwm(uint8_t value)
    {
        if(value > MIN_PWM){
            DDRD |= (1<<3);
        }else{
            DDRD &= ~(1<<3);
            PORTD &= ~(1<<3);
        }

        OCR2B = value;
    }

};

#endif
