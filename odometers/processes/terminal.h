#ifndef TERMINAL_H
#define TERMINAL_H

#include <string.h>
#include <avr/io.h>
#include <stdlib.h>

#include "mediator.h"

typedef int16_t Ftype;

template <
    typename Serial,
    typename Time
    >
class Terminal
{
public:
    void init()
    {
        state_ = state_wait_begin_symbol;
        string_length_ = 0;
    }

    void run()
    {
        uint16_t value = 0;

        switch(state_)
        {
        case state_wait_begin_symbol:
            if(Serial::available() > 0){
                char ch = Serial::read();
                if(ch == ':'){
                    state_ = state_get_line;
                    string_length_ = 0;
                }
            }
            break;

        case state_get_line:
            if(Serial::available() > 0){
                char ch = Serial::read();
                if(ch == '\n'){ //end of line
                    buffer_[string_length_] = '\0';

                    //function
                    uint8_t function = buffer_[0] - '0';

                    //paramer
                    parameter_ = buffer_[1] - '0';

                    //side
                    side_ = buffer_[2] - '0';

                    //data
                    if(function == function_set){
                        data_ = atoi(buffer_+3);
                    }

                    //next state
                    state_ = state_set + function;

                }else{
                    buffer_[string_length_] = ch;
                    string_length_++;
                }
            }
            break;


        case state_set:
            switch(parameter_)
            {
            case param_i_max:
                Mediator::pid[side_].i_max = data_;
                break;
            case param_i_min:
                Mediator::pid[side_].i_min = data_;
                break;
            case param_p_gain:
                Mediator::pid[side_].p_gain = data_;
                break;
            case param_i_gain:
                Mediator::pid[side_].i_gain = data_;
                break;
            case param_d_gain:
                Mediator::pid[side_].d_gain = data_;
                break;
            case param_rps:
                Mediator::set_rps[side_] = data_;
                break;
            }
            state_ = state_wait_begin_symbol;
            break;

        case state_get:
            switch(parameter_)
            {
            case param_i_max:
                value = Mediator::pid[side_].i_max;
                break;
            case param_i_min:
                value = Mediator::pid[side_].i_min;
                break;
            case param_p_gain:
                value = Mediator::pid[side_].p_gain;
                break;
            case param_i_gain:
                value = Mediator::pid[side_].i_gain;
                break;
            case param_d_gain:
                value = Mediator::pid[side_].d_gain;
                break;
            case param_rps:
                value = Mediator::real_rps[side_];
                break;
            }

            itoa(value, buffer_, 10);
            for(int i = 0; ;i++){
                if(buffer_[i] == '\0'){
                    buffer_[i] = '\n';
                    buffer_[i+1] = '\0';
                    break;
                }
            }
            Serial::write(buffer_, strlen(buffer_));
            state_ = state_transfering;
            break;

        case state_transfering:
            if(Serial::isWriten()){
                state_ = state_wait_begin_symbol;
            }
            break;
        }
    }

private:
    uint8_t state_;
    enum{
        state_wait_begin_symbol,
        state_get_line,

        state_set,
        state_get,

        state_transfering,
        state_receiving_string
    };

    enum{
        function_set,
        function_get
    };

    enum{
        param_i_max,
        param_i_min,
        param_p_gain,
        param_i_gain,
        param_d_gain,
        param_rps
    };

    enum { buffer_size_ = 32 };
    char buffer_[buffer_size_];
    uint16_t string_length_;

    uint8_t side_; //left or right
    uint8_t parameter_;
    int16_t data_;

};

#endif
