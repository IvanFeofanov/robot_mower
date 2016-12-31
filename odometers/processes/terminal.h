#ifndef TERMINAL_H
#define TERMINAL_H

#include <string.h>
#include <avr/io.h>

#include "process.h"
#include "messages.h"

#include "types/diff_control.h"
#include "../utils/pid.h"

#include "../utils/menu_item.h"
#include "to_state_menu_item.h"

template <
    typename Serial,
    typename Time,
    uint16_t COMMAND_CHANGED,
    uint16_t PID_COEFF_CHANGED,
    uint16_t SPEED_OBTAINED
    >
class Terminal : public Process
{
public:
    void init()
    {
        state_ = 0;
        string_length_ = 0;

        left_speed_feedback_ = 0;
        right_speed_feedback_ = 0;

        //make menu
        root_item_          = MenuItem(0, "Main menu");
        pid_item_           = MenuItem('p', "PID");
        l_pid_item_         = MenuItem('l', "Left");
        set_l_pid_item_     = ToStateMenuItem('s', "Set", &state_, state_set_l_pid);
        get_l_pid_item_     = ToStateMenuItem('g', "Get", &state_, state_get_l_pid);
        r_pid_item_         = MenuItem('r', "Right");
        set_r_pid_item_     = ToStateMenuItem('s', "Set", &state_, state_set_r_pid);
        get_r_pid_item_     = ToStateMenuItem('g', "Get", &state_, state_get_r_pid);
        speed_item_         = MenuItem('s', "Speed");
        l_speed_item_       = MenuItem('l', "Left");
        set_l_speed_item_   = ToStateMenuItem('s', "Set", &state_, state_set_l_speed);
        get_l_speed_item_   = ToStateMenuItem('g', "Get", &state_, state_get_l_speed);
        r_speed_item_       = MenuItem('r', "Right");
        set_r_speed_item_   = ToStateMenuItem('s', "Set", &state_, state_set_r_speed);
        get_r_speed_item_   = ToStateMenuItem('g', "Get", &state_, state_get_r_speed);
    }

    void run()
    {
        switch(state_)
        {
        case wait_begin_symbol:
            if(Serial::available() > 0){
                char ch = Serial::read();
                if(ch == ':'){
                    state_ = wait_begin_symbol;
                    string_length_ = 0;
                }
            }
            break;
        //
        // case get_command:
        //     if(Serial::available() > 0){
        //         command_ = Serial::read();
        //         if(command_ == 's' || command_ == 'g'){
        //             state_ = get_object;
        //         }else{
        //             state_ = wait_begin_symbol;
        //         }
        //     }
        //     break;
        //
        // case get_object:
        //     if(Serial::available() > 0){
        //         object_ = Serial::read();
        //         switch(command_)
        //         {
        //         case 's':
        //             if(object_ == 'p'){
        //                 state_ = set_pid_coef;
        //             }else if(object_ == 's'){
        //                 state_ = set_speed;
        //             }else{
        //                 state_ = wait_begin_symbol;
        //             }
        //             break;
        //
        //         case 'g':
        //             if(object_ == 'p')
        //                 state_ = wait_begin_symbol;
        //             else if(object_ == 'f')
        //                 state_ = send_speed;
        //             else
        //                 state_ = wait_begin_symbol;
        //             break;
        //
        //         default:
        //             state_ = wait_begin_symbol;
        //         }
        //
        //         string_length_ = 0;
        //     }
        //     break;
        //
        // case set_pid_coef:
        //     if(Serial::available() > 0){
        //         char ch = Serial::read();
        //         if(ch == '\n'){
        //             buffer_[string_length_] = '\0';
        //             int i_max, i_min, p, i, d;
        //             sscanf(buffer_, "imax=%d imin=%d p=%d i=%d d=%d",
        //                     &i_max, &i_min, &p, &i, &d);
        //             pid_ = Pid(i_max, i_min, p, i, d);
        //
        //             Messages::send(PID_COEFF_CHANGED, &pid_);
        //             state_ = wait_begin_symbol;
        //         }else{
        //             buffer_[string_length_] = ch;
        //         }
        //         string_length_++;
        //     }
        //     break;
        //
        // case set_speed:
        //     if(Serial::available() > 0){
        //         char ch = Serial::read();
        //         if(ch == '\n'){
        //             buffer_[string_length_] = '\0';
        //             int l, r;
        //             sscanf(buffer_, "l=%d r=%d", &l, &r);
        //             diff_control_.left_speed = l;
        //             diff_control_.right_speed = r;
        //
        //             Messages::send(COMMAND_CHANGED, &diff_control_);
        //             state_ = wait_begin_symbol;
        //         }else{
        //             buffer_[string_length_] = ch;
        //         }
        //         string_length_++;
        //     }
        //     break;

        case send_speed:
            if(Serial::isWriten()){
                sprintf(buffer_, "ls=%d rs=%d\n",
                        (int)left_speed_feedback_, (int)right_speed_feedback_);
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

        float* speed;
        if(Messages::get(SPEED_OBTAINED, (void**)(&speed)))
        {
            left_speed_feedback_ = *speed;
            right_speed_feedback_ = *speed;
        }
    }


private:
    uint8_t state_;
    enum {
        wait_begin_symbol,

        state_set_l_pid,
        state_get_l_pid,
        state_set_r_pid,
        state_get_r_pid,
        state_set_l_speed,
        state_get_l_speed,
        state_set_r_speed,
        state_get_r_speed,

        send_speed,
        wait_transfering

        // parsing,
        // send_counter,
        // wait_transfering
    };

    enum { buffer_size_ = 128 };
    char buffer_[buffer_size_];
    uint16_t string_length_;

    //menu
    MenuItem        root_item_;
    MenuItem        pid_item_;
    MenuItem        l_pid_item_;
    ToStateMenuItem set_l_pid_item_;
    ToStateMenuItem get_l_pid_item_;
    MenuItem        r_pid_item_;
    ToStateMenuItem set_r_pid_item_;
    ToStateMenuItem get_r_pid_item_;
    MenuItem        speed_item_;
    MenuItem        l_speed_item_;
    ToStateMenuItem set_l_speed_item_;
    ToStateMenuItem get_l_speed_item_;
    MenuItem        r_speed_item_;
    ToStateMenuItem set_r_speed_item_;
    ToStateMenuItem get_r_speed_item_;

    MenuItem* current_menu_item_;

    DiffControl diff_control_;
    Pid pid_;

    float left_speed_feedback_;
    float right_speed_feedback_;
};

#endif
