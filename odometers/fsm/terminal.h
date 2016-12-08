#ifndef TERMINAL_H
#define TERMINAL_H

#include <string.h>

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
        parsing
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
            PORTB |= (1<<5);
            char ch = Serial::read();
            if(ch == ':'){
                state_ = get_byte;
                PORTB &= ~(1<<5);
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
        if(sscanf(buffer_, "pwm=%d", &pwm_)){
            string_length_ = 0;
            state_ = wait_begin_symbol;
            Drive::setPwm(pwm_);
        }
        break;
    }
}

#endif
