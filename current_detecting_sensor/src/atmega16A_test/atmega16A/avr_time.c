#include "avr_time.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define CLOCK_TICKS_PER_MS F_CPU / 1000UL
//timer is 8 bits and is prescaled by 64
#define CLOCK_TICKS_PER_TIMER_OVERFLOW 64UL * 256UL

volatile static uint32_t timer_ticks = 0;
volatile static uint32_t overflow_ms = 0;

//The timer overflow interrupt handler
ISR(TIMER0_OVF_vect)
{
    avr_time_overflow_handler();
}

void avr_time_overflow_handler()
{
    timer_ticks += CLOCK_TICKS_PER_TIMER_OVERFLOW;
    for(; timer_ticks > CLOCK_TICKS_PER_MS; timer_ticks -= CLOCK_TICKS_PER_MS){
        overflow_ms++;
    }
}

void avr_time_init()
{
    TCCR0 |= (1<<CS01) | (1<<CS00); // Clk/64
    TIMSK |= (1<<TOIE0); //overflow interrupt enable
}

uint32_t avr_time_now()
{
    uint32_t time_now;

    cli();
    time_now = overflow_ms;
    sei();

    return time_now;
}
