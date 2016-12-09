#ifndef TERMINAL_H
#define TERMINAL_H

#include <string.h>

volatile extern uint32_t cntr;

template <
    typename Serial,
    typename Drive
    >
class Terminal
{
public:
    void init();
    void exec();
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
};

template<typename Serial, typename Drive>
void Terminal<Serial, Drive>::init()
{
    state_ = 0;
    string_length_ = 0;
}

template<typename Serial, typename Drive>
void Terminal<Serial, Drive>::exec()
{
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
        if(sscanf(buffer_, "%d", &pwm_)){
            Drive::setPwm(pwm_);
        }

        string_length_ = 0;
        state_ = send_counter;

        break;

    case send_counter:
        if(Serial::isWriten()){
            sprintf(buffer_, "counter=%d\n", DriveMotors::getCounter());
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

#endif