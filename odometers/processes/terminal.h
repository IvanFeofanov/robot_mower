#ifndef TERMINAL_H
#define TERMINAL_H

#include <string.h>

#include "process.h"
#include "messages.h"

#include "../utils/pid.h"

template <
    typename Serial,
    typename Time,
    typename Drive
    >
class Terminal : public Process
{
public:
    void init();
    void run();
    void handleMessages();

private:
    uint8_t state_;
    enum {
        wait_begin_symbol,
        get_byte,
        parsing,
        send_counter,
        wait_transfering
    };

    enum { buffer_size_ = 128 };
    char buffer_[buffer_size_];
    uint16_t string_length_;

    uint8_t pwm_;

    uint32_t last_time_;
    uint16_t rps_;
    uint16_t speed_;

    Pid pid_;
};

template<typename Serial, typename Time, typename Drive>
void Terminal<Serial, Time, Drive>::init()
{
    state_ = 0;
    string_length_ = 0;
    last_time_ = 0;
    rps_ = 0;
    speed_ = 0;
}

template<typename Serial, typename Time, typename Drive>
void Terminal<Serial, Time, Drive>::run()
{
    if(Time::now() - last_time_ >= 100)
    {
        rps_ = DriveMotors::getCounter()+1;
        double value = pid_.update((double)(speed_), (double)(rps_));
        uint8_t pwm;
        if(value < 0.0){
            pwm = 0;
        }else if(value > 255.0){
            pwm = 255;
        }else{
            pwm = (uint8_t)(value);
        }

        Drive::setPwm(pwm);
        last_time_ = Time::now();
    }

    switch(state_)
    {
    case wait_begin_symbol:
        if(Serial::available() > 0){
            char ch = Serial::read();
            if(ch == ':'){
                state_ = get_byte;
            }
        }
        break;

    case get_byte:
        if(Serial::available() > 0){
            char ch = Serial::read();
            if(ch == '\n'){
                buffer_[string_length_] = '\0';
                state_ = parsing;
            }else{
                buffer_[string_length_] = ch;
            }
            string_length_++;
        }
        break;

    case parsing:
        if(sscanf(buffer_, "%d", &speed_)){
            Drive::setPwm(pwm_);
            if(speed_ > 20){
                MsgHandler::send(0, 0);
            }
        }

        string_length_ = 0;
        state_ = send_counter;

        break;

    case send_counter:
        if(Serial::isWriten()){
            sprintf(buffer_, "counter=%d\n", rps_);
            Serial::write(buffer_, strlen(buffer_));
            state_ = wait_transfering;
        }
        break;

    case wait_transfering:
        if(Serial::isWriten()){
            state_ = wait_begin_symbol;
        }
        break;
    }
}

template<typename Serial, typename Time, typename Drive>
void Terminal<Serial, Time, Drive>::handleMessages()
{
}

#endif
