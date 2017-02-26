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
        left_calibration_flag_ = false;
        right_calibration_flag_ = false;

        AdcLeftPot::setSlot(&left_value_, 1);
        AdcRightPot::setSlot(&right_value_, 1);

        AdcLeftPot::startCapture();
        AdcRightPot::startCapture();
    }

    void run(BumperMsg* bumper_msg)
    {
        if(AdcLeftPot::isCaptureComplate()){
            if(bumper_msg->is_calibration){
                if(left_calibration_flag_){
                    if(left_value_ > bumper_msg->left_max){
                        bumper_msg->left_max = left_value_;
                    }
                    if(left_value_ < bumper_msg->left_min){
                        bumper_msg->left_min = left_value_;
                    }
                }else{
                    bumper_msg->left_max = left_value_;
                    bumper_msg->left_min = left_value_;
                }
            }else{ //normal mode
                bumper_msg->left = map(left_value_, bumper_msg->left_min,
                    bumper_msg->left_max, 0, 255);
            }

            left_calibration_flag_ = bumper_msg->is_calibration;

            AdcLeftPot::startCapture();
        }

        if(AdcRightPot::isCaptureComplate()){
            if(bumper_msg->is_calibration){
                if(right_calibration_flag_){
                    if(right_value_ > bumper_msg->right_max){
                        bumper_msg->right_max = right_value_;
                    }
                    if(right_value_ < bumper_msg->right_min){
                        bumper_msg->right_min = right_value_;
                    }
                }else{
                    bumper_msg->right_max = right_value_;
                    bumper_msg->right_min = right_value_;
                }
            }else{ //normal mode
                bumper_msg->right = map(right_value_, bumper_msg->right_min,
                    bumper_msg->right_max, 0, 255);
            }

            right_calibration_flag_ = bumper_msg->is_calibration;

            AdcRightPot::startCapture();
        }

    }

private:
    uint16_t left_value_;
    uint16_t right_value_;

    bool left_calibration_flag_;
    bool right_calibration_flag_;
};

#endif
