#ifndef MOTORS_H
#define MOTORS_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
}

#define M_L_1_HIGH {PORTD |= (1<<7);}
#define M_L_1_LOW {PORTD &= ~(1<<7);}
#define M_L_2_HIGH {PORTB |= (1<<0);}
#define M_L_2_LOW {PORTB &= ~(1<<0);}

#define M_R_1_HIGH {PORTB |= (1<<1);}
#define M_R_1_LOW {PORTB &= ~(1<<1);}
#define M_R_2_HIGH {PORTB |= (1<<2);}
#define M_R_2_LOW {PORTB &= ~(1<<2);}

#define LEFT_PWM (OCR0A)
#define RIGHT_PWM (OCR0B)

class Motors
{
public:
    static inline void init()
    {
        //ports
        DDRD |= (1<<5) | (1<<6); // OC0B, OC0A
        DDRD |= (1<<7); //M11
        DDRB |= (1<<0) | (1<<1) | (1<<2); //M12, M21, M22

        //pwm
        TCCR0A |= (1<<COM0A1) | // OCR0A non-inverting mode
                  (1<<COM0B1) | // OCR0B non-inverting mode
                  (1<<WGM01) | (1<<WGM00); //Fast pwm

        TCCR0B |= (1<<CS00); //clk = F_CPU

        LEFT_PWM = 0;
        RIGHT_PWM = 0;

        setDirection(STOP, STOP);
    }

    static inline void setPwm(uint8_t left, uint8_t right)
    {
        LEFT_PWM = left;
        RIGHT_PWM = right;
    }

    static void setDirection(int8_t left, int8_t right)
    {
        //left
        M_L_1_LOW;
        M_L_2_LOW;
        if(left > 0)
            M_L_1_HIGH;
        if(left < 0)
            M_L_2_HIGH;

        //right
        M_R_1_LOW;
        M_R_2_LOW;
        if(right > 0)
            M_R_1_HIGH;
        if(right < 0)
            M_R_2_HIGH;

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
