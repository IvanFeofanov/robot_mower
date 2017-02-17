#ifndef BUMPER_H
#define BUMPER_H

#include "types.h"
#include "map.h"

template<
    typename Time,
    typename AdcLeftPot,
    typename AdcRightPot
    >
class Bumper
{
public:
    void init()
    {
        left_value_ = 0;
        right_value_ = 0;
        last_calibration_flag_ = false;

        AdcLeftPot::setSlot(&left_value_, 1);
        AdcRightPot::setSlot(&right_value_, 1);

        AdcLeftPot::startCapture();
        AdcRightPot::startCapture();
    }

    void run(BumperMsg* bumper_msg)
    {
        uint16_t tmp_left = 0;
        uint16_t tmp_right = 0;

        if(AdcLeftPot::isCaptureComplate()){
            tmp_left = left_value_;
            AdcLeftPot::startCapture();
        }

        if(AdcRightPot::isCaptureComplate()){
            tmp_right = right_value_;
            AdcRightPot::startCapture();
        }

        if(bumper_msg->is_calibration){ // calibration mode
            if(last_calibration_flag_){
                // left
                if(tmp_left > bumper_msg->left_max){
                    bumper_msg->left_max = tmp_left;
                }
                if(tmp_left < bumper_msg->left_min){
                    bumper_msg->left_min = tmp_left;
                }

                //right
                if(tmp_right > bumper_msg->right_max){
                    bumper_msg->right_max = tmp_right;
                }
                if(tmp_right < bumper_msg->right_min){
                    bumper_msg->right_min = tmp_right;
                }

            }else{ // if first calibration
                // left
                bumper_msg->left_max = tmp_left;
                bumper_msg->left_min = tmp_left;

                // right
                bumper_msg->right_max = tmp_right;
                bumper_msg->right_min = tmp_right;
            }
        }else{ // normal mode
            bumper_msg->left = map(tmp_left, bumper_msg->left_min,
                    bumper_msg->left_max, 0, 255);
            bumper_msg->right = map(tmp_right, bumper_msg->right_min,
                    bumper_msg->right_max, 0, 255);
        }

        last_calibration_flag_ = bumper_msg->is_calibration;

    }

private:
    uint16_t left_value_;
    uint16_t right_value_;

    bool last_calibration_flag_;
};

#endif
