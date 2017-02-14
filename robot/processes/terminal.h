#ifndef TERMINAL_H
#define TERMINAL_H

#include <string.h>
#include <stdio.h>
#include <avr/io.h>

#include "../utils/tree_menu/menu_item.h"
#include "../utils/tree_menu/tree_menu.h"
#include "../utils/tree_menu/to_state_menu_item.h"

#include "types.h"

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

        //make menu
        root_item_             = MenuItem(0, "Main menu");
        print_motors_item_     =  ToStateMenuItem('0', "print motors",
                &state_, state_print_drive_motors);
        print_toi_item_        =  ToStateMenuItem('1', "print toi",
                &state_, state_print_toi);
        bumper_item_           =  MenuItem('2', "bumper");
        print_bumper_item_     =   ToStateMenuItem('0', "print bumper",
                &state_, state_print_bumper);
        calib_bumper_item_     =   ToStateMenuItem('1', "calib bumper",
                &state_, state_calib_bumper);

        root_item_.addChild(&print_motors_item_);
        print_motors_item_.addNext(&print_toi_item_);
        print_toi_item_.addNext(&bumper_item_);
        bumper_item_.addChild(&print_bumper_item_);
        print_bumper_item_.addNext(&calib_bumper_item_);

        menu_ = TreeMenu(&root_item_, &state_, state_print_help);
    }

    void run(uint8_t* status,
            MowerMsg* mower_msg,
            DriveMotorsMsg* motors_msg,
            BumperMsg* bumper_msg)
    {
        uint32_t toi = getToi();

        switch(state_)
        {
        case state_wait_begin_symbol:
            if(Serial::available() > 0){
                char ch = Serial::read();
                if(ch == ':'){
                    state_ = state_get_symbol;
                }
            }
            break;

        case state_get_symbol:
            if(Serial::available() > 0){
                char ch = Serial::read();
                if(!menu_.select(ch)){
                    state_ = state_wait_begin_symbol;
                }
            }
            break;

        case state_print_drive_motors:
            sprintf(buffer_,
                    "lss=%d rss=%d lrs=%d rrs=%d lc=%d rc=%d\n",
                    motors_msg->left_set_speed, motors_msg->right_set_speed,
                    motors_msg->left_real_speed, motors_msg->right_real_speed,
                    motors_msg->left_counter, motors_msg->right_counter);
            Serial::write(buffer_, strlen(buffer_));
            state_ = state_transfering;
            transfering_next_state_ = state_wait_begin_symbol;
            break;

        case state_print_toi:
            sprintf(buffer_, "toi=%dms\n", toi);
            Serial::write(buffer_, strlen(buffer_));
            state_ = state_transfering;
            transfering_next_state_ = state_wait_begin_symbol;
            break;

        case state_print_bumper:
            sprintf(buffer_, "left=%d right=%d\n", bumper_msg->left, bumper_msg->right);
            Serial::write(buffer_, strlen(buffer_));
            state_ = state_transfering;
            transfering_next_state_ = state_wait_begin_symbol;
            break;

        case state_calib_bumper:
            bumper_msg->is_calibration = true;

            sprintf(buffer_, "1-save\n");
            Serial::write(buffer_, strlen(buffer_));
            state_ = state_transfering;
            transfering_next_state_ = state_wait_end_calib;
            break;

        case state_wait_end_calib:
            state_ = state_receiving_string;
            receiving_next_state_ = state_end_calib;
            string_length_ = 0;
            break;

        case state_end_calib:
            bumper_msg->is_calibration = false;

            sprintf(buffer_, "l min/max %d/%d, r min/max %d/%d\n",
                    bumper_msg->left_min, bumper_msg->left_max,
                    bumper_msg->right_min, bumper_msg->right_max);
            Serial::write(buffer_, strlen(buffer_));
            state_ = state_transfering;
            transfering_next_state_ = state_wait_begin_symbol;
            break;

        case state_print_help:
            menu_.getHelpLine(buffer_);
            if(buffer_[0] == '\0'){
                state_ = state_wait_begin_symbol;
            }else{
                strcat(buffer_, "\n");
                Serial::write(buffer_, strlen(buffer_));
                state_ = state_transfering;
                transfering_next_state_ = state_print_help;
            }
            break;

        case state_transfering:
            if(Serial::isWriten()){
                state_ = transfering_next_state_;
            }
            break;

        case state_receiving_string:
            if(Serial::available() > 0){
                char ch = Serial::read();
                if(ch == '\n'){
                    if(string_length_ == 0)
                        break;
                    buffer_[string_length_] = '\0';
                    state_ = receiving_next_state_;
                }else{
                    buffer_[string_length_] = ch;
                    string_length_++;
                }
            }
            break;
        }
    }

private:
    //возвращает время одной итерации цикла конечного автомата
    //(time of iteration)
    uint32_t getToi()
    {
        static uint32_t last_time = 0;
        uint32_t toi = Time::now() - last_time;
        last_time = Time::now();
        return toi;
    }

private:
    uint8_t state_;

    enum {
        state_wait_begin_symbol,
        state_get_symbol,

        state_print_drive_motors,
        state_print_toi,
        state_print_bumper,

        state_calib_bumper,
        state_wait_end_calib,
        state_end_calib,

        state_print_help,
        state_transfering,
        state_receiving_string
    };

    uint8_t transfering_next_state_;
    uint8_t receiving_next_state_;

    enum { buffer_size_ = 128 };
    char buffer_[buffer_size_];
    uint16_t string_length_;

    //menu
    TreeMenu menu_;
    MenuItem root_item_;

    ToStateMenuItem print_motors_item_;
    ToStateMenuItem print_toi_item_;
    MenuItem        bumper_item_;
    ToStateMenuItem print_bumper_item_;
    ToStateMenuItem calib_bumper_item_;

};

#endif
