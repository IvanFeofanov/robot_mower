#ifndef ODOMETERS_H
#define ODOMETERS_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
}

#define INTERRUPTS_PER_ROTATION (2)
#define MAX_TIME (5000) //uS

class Odometers
{
public:
    typedef uint32_t CounterType;

    static void init()
    {
        right_counter_  = 0;
        left_counter_   = 0;
        left_time_      = 0;
        right_time_     = 0;

        //port direction
        DDRD &= ~(1<<2 | 1<<3); // INT0, INT1

        //external interrupt
        EICRA = (1<<ISC10) | (1<<ISC00); // Any logical change generates interrupts
        EIMSK = (1<<INT0)  | (1<<INT1);

        //timer
        TCCR1A = (0<<WGM11) | (0<<WGM10);
        TCCR1B = (1<<WGM12)| // pwm mode - CTC
                 (1<<CS10); // Clk/1

        OCR1A = 799; // 20000 hz
        TIMSK1 = (1<<OCIE1A); // Output compare A match interrupt Enable
    }

    static CounterType left_counter()
    {
        CounterType tmp = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            tmp = left_counter_;
        }
        return tmp;
    }

    static CounterType right_counter()
    {
        CounterType tmp = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            tmp = right_counter_;
        }
        return tmp;
    }

    static void reset_right_counter()
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            left_counter_ = 0;
        }
    }

    static void reset_left_counter()
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            right_counter_ = 0;
        }
    }

    static bool left_rps(uint16_t* rps)
    {
        uint32_t time;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            time = left_time_;
            left_time_ = 0;
        }

        //if have not information about rps
        if(time == 0){
            return false;
        }

        *rps = 1000000UL / (time * 50 * 2);

        return true;
    }

    static bool right_rps(uint16_t* rps)
    {
        uint32_t time;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            time = right_time_;
            right_time_ = 0;
        }

        //if have not information about rps
        if(time == 0){
            return false;
        }

        *rps = 1000000UL / (time * 50 * 2);

        return true;
    }

public:
    static void left_odometer_interrupt()
    {
        static uint8_t n = 0;

        n++;
        if(n == INTERRUPTS_PER_ROTATION){
            left_counter_++;
            n = 0;
        }

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if(is_left_tc_overflow_){
                left_time_ = MAX_TIME;
                is_left_tc_overflow_ = false;
            }else{
                left_time_ = left_tc_; // (left_time_ + left_tc_) / 2;
            }

            left_tc_ = 0;
        }

    }

    static void right_odometer_interrupt()
    {
        static uint8_t n = 0;

        n++;
        if(n == INTERRUPTS_PER_ROTATION){
            right_counter_++;
            n = 0;
        }

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            if(is_right_tc_overflow_){
                right_time_ = MAX_TIME;
                is_right_tc_overflow_ = false;
            }else{
                right_time_ = right_tc_; // (left_time_ + left_tc_) / 2;
            }

            right_tc_ = 0;
        }

    }

    static void timer_interrupt()
    {
        left_tc_++;
        if(left_tc_ >= MAX_TIME){
            is_left_tc_overflow_ = true;
            left_time_ = MAX_TIME;
            left_tc_ = 0;
        }

        right_tc_++;
        if(right_tc_ >= MAX_TIME){
            is_right_tc_overflow_ = true;
            right_time_ = MAX_TIME;
            right_tc_ = 0;
        }
    }

private:
    volatile static CounterType left_counter_;
    volatile static CounterType right_counter_;

    volatile static uint16_t left_tc_;
    volatile static bool     is_left_tc_overflow_;
    volatile static uint16_t left_time_;
    volatile static uint16_t right_tc_;
    volatile static bool     is_right_tc_overflow_;
    volatile static uint16_t right_time_;

};

#endif
