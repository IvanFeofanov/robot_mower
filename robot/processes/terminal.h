#ifndef TERMINAL_H
#define TERMINAL_H

#include <string.h>
#include <avr/io.h>

// #include "../utils/menu_item.h"
// #include "../utils/tree_menu.h"
// #include "to_state_menu_item.h"
//
template <
    typename Serial,
    typename Time
    >
class Terminal
{
public:
    void init()
    {
        // state_ = state_wait_begin_symbol;
        // string_length_ = 0;
        //
        // left_speed_feedback_ = 0;
        // right_speed_feedback_ = 0;
        //
        // //make menu
        // root_item_          = MenuItem(0, "Main menu");
        // pid_item_           =  MenuItem('p', "PID");
        // l_pid_item_         =   MenuItem('l', "Left");
        // set_l_pid_item_     =    ToStateMenuItem('s', "Set", &state_, state_set_l_pid);
        // get_l_pid_item_     =    ToStateMenuItem('g', "Get", &state_, state_get_l_pid);
        // r_pid_item_         =   MenuItem('r', "Right");
        // set_r_pid_item_     =    ToStateMenuItem('s', "Set", &state_, state_set_r_pid);
        // get_r_pid_item_     =    ToStateMenuItem('g', "Get", &state_, state_get_r_pid);
        // speed_item_         =  MenuItem('s', "Speed");
        // l_speed_item_       =   MenuItem('l', "Left");
        // set_l_speed_item_   =    ToStateMenuItem('s', "Set", &state_, state_set_l_speed);
        // get_l_speed_item_   =    ToStateMenuItem('g', "Get", &state_, state_get_l_speed);
        // r_speed_item_       =   MenuItem('r', "Right");
        // set_r_speed_item_   =    ToStateMenuItem('s', "Set", &state_, state_set_r_speed);
        // get_r_speed_item_   =    ToStateMenuItem('g', "Get", &state_, state_get_r_speed);
        //
        // root_item_.addChild(&pid_item_);
        // pid_item_.addChild(&l_pid_item_);
        // l_pid_item_.addChild(&set_l_pid_item_);
        // set_l_pid_item_.addNext(&get_l_pid_item_);
        // l_pid_item_.addNext(&r_pid_item_);
        // r_pid_item_.addChild(&set_r_pid_item_);
        // set_r_pid_item_.addNext(&get_r_pid_item_);
        //
        // pid_item_.addNext(&speed_item_);
        // speed_item_.addChild(&l_speed_item_);
        // l_speed_item_.addChild(&set_l_speed_item_);
        // set_l_speed_item_.addNext(&get_l_speed_item_);
        // l_speed_item_.addNext(&r_speed_item_);
        // r_speed_item_.addChild(&set_r_speed_item_);
        // set_r_speed_item_.addNext(&get_r_speed_item_);
        //
        // menu_ = TreeMenu(&root_item_, &state_, state_print_help);
    }

    void run()
    {
        // int speed;
        //
        // switch(state_)
        // {
        // case state_wait_begin_symbol:
        //     if(Serial::available() > 0){
        //         char ch = Serial::read();
        //         if(ch == ':'){
        //             state_ = state_get_symbol;
        //         }
        //     }
        //     break;
        //
        // case state_get_symbol:
        //     if(Serial::available() > 0){
        //         char ch = Serial::read();
        //         if(!menu_.select(ch)){
        //             state_ = state_wait_begin_symbol;
        //         }
        //     }
        //     break;
        //
        // case state_set_l_pid:
        //     state_ = state_receiving_string;
        //     receiving_next_state_ = state_set_l_pid_parsing;
        //     string_length_ = 0;
        //     break;
        //
        // case state_set_l_pid_parsing:
        //     sscanf(buffer_, "imax=%f imin=%f p=%f i=%f d=%f",
        //             &pid_.i_max, &pid_.i_min, &pid_.p_gain, &pid_.i_gain, &pid_.d_gain);
        //     Messages::send(PID_COEFF_CHANGED, (void*)(&pid_));
        //     state_ = state_wait_begin_symbol;
        //     break;
        //
        // case state_get_l_pid:
        //     sprintf(buffer_, "imax=%f imin=%f p=%f i=%f d=%f\n", pid_.i_max,
        //             pid_.i_min, pid_.p_gain,
        //             pid_.i_gain, pid_.d_gain);
        //     Serial::write(buffer_, strlen(buffer_));
        //     state_ = state_transfering;
        //     transfering_next_state_ = state_wait_begin_symbol;
        //     break;
        //
        // case state_set_r_pid:
        //     break;
        //
        // case state_get_r_pid:
        //     break;
        //
        // case state_set_l_speed:
        //     state_ = state_receiving_string;
        //     receiving_next_state_ = state_set_l_speed_parsing;
        //     string_length_ = 0;
        //     break;
        //
        // case state_set_l_speed_parsing:
        //     sscanf(buffer_, "%d", &speed);
        //     diff_control_.left_speed = speed;
        //     Messages::send(COMMAND_CHANGED, (void*)(&diff_control_));
        //     state_ = state_wait_begin_symbol;
        //     break;
        //
        // case state_get_l_speed:
        //     sprintf(buffer_, "%d\n", (int)left_speed_feedback_);
        //     Serial::write(buffer_, strlen(buffer_));
        //     state_ = state_transfering;
        //     transfering_next_state_ = state_wait_begin_symbol;
        //     break;
        //
        // case state_set_r_speed:
        //     state_ = state_receiving_string;
        //     receiving_next_state_ = state_set_r_speed_parsing;
        //     break;
        //
        // case state_set_r_speed_parsing:
        //     sscanf(buffer_, "%d", &speed);
        //     diff_control_.right_speed = speed;
        //     Messages::send(COMMAND_CHANGED, (void*)(&diff_control_));
        //     state_ = state_wait_begin_symbol;
        //
        //     break;
        //
        // case state_get_r_speed:
        //     sprintf(buffer_, "%d\n", (int)right_speed_feedback_);
        //     Serial::write(buffer_, strlen(buffer_));
        //     state_ = state_transfering;
        //     transfering_next_state_ = state_wait_begin_symbol;
        //     break;
        //
        // case state_print_help:
        //     menu_.getHelpOneLine(buffer_);
        //     if(buffer_[0] == '\0'){
        //         state_ = state_get_symbol;
        //     }else{
        //         strcat(buffer_, "\n");
        //         Serial::write(buffer_, strlen(buffer_));
        //         state_ = state_transfering;
        //         transfering_next_state_ = state_print_help;
        //     }
        //     break;
        //
        // case state_transfering:
        //     if(Serial::isWriten()){
        //         state_ = transfering_next_state_;
        //     }
        //     break;
        //
        // case state_receiving_string:
        //     if(Serial::available() > 0){
        //         char ch = Serial::read();
        //         if(ch == '\n'){
        //             if(string_length_ == 0)
        //                 break;
        //             buffer_[string_length_] = '\0';
        //             state_ = receiving_next_state_;
        //         }else{
        //             buffer_[string_length_] = ch;
        //             string_length_++;
        //         }
        //     }
        //     break;
        // }
    }

private:
    // uint8_t state_;
    // enum {
    //     state_wait_begin_symbol,
    //     state_get_symbol,
    //
    //     state_set_l_pid,
    //     state_set_l_pid_parsing,
    //     state_get_l_pid,
    //     state_set_r_pid,
    //     state_get_r_pid,
    //     state_set_l_speed,
    //     state_set_l_speed_parsing,
    //     state_get_l_speed,
    //     state_set_r_speed_parsing,
    //     state_set_r_speed,
    //     state_get_r_speed,
    //     state_print_help,
    //
    //     state_transfering,
    //     state_receiving_string
    // };
    //
    // uint8_t transfering_next_state_;
    // uint8_t receiving_next_state_;
    //
    // enum { buffer_size_ = 128 };
    // char buffer_[buffer_size_];
    // uint16_t string_length_;
    //
    // //menu
    // MenuItem        root_item_;
    // MenuItem        pid_item_;
    // MenuItem        l_pid_item_;
    // ToStateMenuItem set_l_pid_item_;
    // ToStateMenuItem get_l_pid_item_;
    // MenuItem        r_pid_item_;
    // ToStateMenuItem set_r_pid_item_;
    // ToStateMenuItem get_r_pid_item_;
    // MenuItem        speed_item_;
    // MenuItem        l_speed_item_;
    // ToStateMenuItem set_l_speed_item_;
    // ToStateMenuItem get_l_speed_item_;
    // MenuItem        r_speed_item_;
    // ToStateMenuItem set_r_speed_item_;
    // ToStateMenuItem get_r_speed_item_;
    // TreeMenu    menu_;
    //
    // Pid pid_;
    //
    // float left_speed_feedback_;
    // float right_speed_feedback_;
};

#endif
