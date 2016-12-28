#ifndef TERMINAL_H
#define TERMINAL_H

#include <string.h>
#include <avr/io.h>

#include "process.h"
#include "messages.h"

#include "types/diff_control.h"

template <
    typename Serial,
    typename Time,
    uint16_t COMMAND_CHANGED
    >
class Terminal : public Process
{
public:
    void init()
    {
        state_ = 0;
        string_length_ = 0;
    }

    void run()
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
            int speed;
            if(sscanf(buffer_, "%d", &speed)){
                diff_control_.left_speed = speed;
                Messages::send(COMMAND_CHANGED, &diff_control_);
            }

            string_length_ = 0;
            state_ = send_counter;

            break;

        case send_counter:
            if(Serial::isWriten()){
                sprintf(buffer_, "counter=%d\n", 42);
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

    void handleMessages()
    {

    }

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

    DiffControl diff_control_;
};

#endif
