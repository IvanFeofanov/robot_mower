#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
 * Based on Atmega16
 */

/* const signed char sigcode [] = { 0, 1 }; */
/* const signed char sigcode [] = { 1,0,-1, 0,1,-1,1,-1, 0,1,-1,1,0,-1, 0,1,-1, 0,1,-1, 0,1,0,-1 }; */
const signed char sigcode [] = { 1,1,-1,-1,1,-1,1,-1,-1,1,-1,1,1,-1,-1,1,-1,-1,1,-1,-1,1,1,-1 };
volatile unsigned int step;

ISR(TIMER2_COMP_vect)
{
    switch(sigcode[step])
    {
    case -1:
        PORTA |= (1<<0);
        PORTA &= ~(1<<1);
        break;

    case 0:
        PORTA &= ~(1<<0);
        PORTA &= ~(1<<1);
        break;

    case 1:
        PORTA &= ~(1<<0);
        PORTA |= (1<<1);
        break;
    }

    if(step + 1 < sizeof(sigcode))
        step++;
    else
        step = 0;
}

void setup()
{
    step = 0;

    //PWM CTC, prescaler - 8
    TCCR2 = (1<<WGM21) | (1<<CS21);
    OCR2 = 207; //9615.38 hz
    TIMSK |= (1<<OCIE2); //output compare match interrupt

    //port
    DDRA |= (1<<0) | (1<<1);
    PORTA &= ~(1<<0);
    PORTA &= ~(1<<1);
    DDRA |= (1<<2);

    sei();
}

void loop()
{
}

int main(void)
{
    setup();

    while(1)
        loop();
}
