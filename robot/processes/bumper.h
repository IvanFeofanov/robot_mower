#ifndef BUMPER_SENSOR_H
#define BUMPER_SENSOR_H

#include "map.h"

template<
    typename AdcLeftPot,
    typename AdcRightPot
    >
class BumperSensor
{
public:
    static void init()
    {
        left_adc_value_ = 0;
        left_min_adc_value_ = 1023;
        left_max_adc_value_ = 0;

        right_adc_value_ = 0;
        right_min_adc_value_ = 1023;
        right_max_adc_value_ = 0;

        is_calibration_mode_ = false;
        last_is_calibration_mode_ = false;

        left_ = 0;
        right_ = 0;

        AdcLeftPot::setSlot(&left_adc_value_, 1);
        AdcRightPot::setSlot(&right_adc_value_, 1);

        AdcLeftPot::startCapture();
        AdcRightPot::startCapture();
    }

    static void update()
    {
        //calibration, first iteration
        if(is_calibration_mode_ && !last_is_calibration_mode_){
            left_min_adc_value_ = left_adc_value_;
            left_max_adc_value_ = left_adc_value_;
            right_min_adc_value_ = right_adc_value_;
            right_max_adc_value_ = right_adc_value_;
        }

        //calibration save changes
        if(!is_calibration_mode_ && last_is_calibration_mode_){
        //     save();
        }

        // left
        if(AdcLeftPot::isCaptureComplate()){
            if(is_calibration_mode_){
                if(left_adc_value_ > left_max_adc_value_){
                    left_max_adc_value_ = left_adc_value_;
                }
                if(left_adc_value_ < left_min_adc_value_){
                    left_min_adc_value_ = left_adc_value_;
                }
            }else{ //normal mode
                left_ = map(left_adc_value_, left_min_adc_value_,
                    left_max_adc_value_, 0, 255);
            }
            AdcLeftPot::startCapture();
        }

        if(AdcRightPot::isCaptureComplate()){
            if(is_calibration_mode_){
                if(right_adc_value_ > right_max_adc_value_){
                    right_max_adc_value_ = right_adc_value_;
                }
                if(right_adc_value_ < right_min_adc_value_){
                    right_min_adc_value_ = right_adc_value_;
                }
            }else{ //normal mode
                right_ = map(right_adc_value_, right_min_adc_value_,
                    right_max_adc_value_, 0, 255);
            }

            AdcRightPot::startCapture();
        }

        last_is_calibration_mode_ = is_calibration_mode_;

    }

    static inline void enableCalibrationMode(bool is_enable)
    {
        is_calibration_mode_ = is_enable;
    }

    static inline bool isCalibrationMode()
    {
        return is_calibration_mode_;
    }

    static inline uint8_t left()
    {
        return left_;
    }

    static inline uint8_t right()
    {
        return right_;
    }

private:
    static uint16_t left_adc_value_;
    static uint16_t left_min_adc_value_;
    static uint16_t left_max_adc_value_;

    static uint16_t right_adc_value_;
    static uint16_t right_min_adc_value_;
    static uint16_t right_max_adc_value_;

    static bool is_calibration_mode_;
    static bool last_is_calibration_mode_;

    static uint8_t left_;
    static uint8_t right_;
};

template<typename AdcLeftPot, typename AdcRightPot>
uint16_t BumperSensor<AdcLeftPot, AdcRightPot>::left_adc_value_;

template<typename AdcLeftPot, typename AdcRightPot>
uint16_t BumperSensor<AdcLeftPot, AdcRightPot>::left_min_adc_value_;

template<typename AdcLeftPot, typename AdcRightPot>
uint16_t BumperSensor<AdcLeftPot, AdcRightPot>::left_max_adc_value_;

template<typename AdcLeftPot, typename AdcRightPot>
uint16_t BumperSensor<AdcLeftPot, AdcRightPot>::right_adc_value_;

template<typename AdcLeftPot, typename AdcRightPot>
uint16_t BumperSensor<AdcLeftPot, AdcRightPot>::right_min_adc_value_;

template<typename AdcLeftPot, typename AdcRightPot>
uint16_t BumperSensor<AdcLeftPot, AdcRightPot>::right_max_adc_value_;

template<typename AdcLeftPot, typename AdcRightPot>
bool BumperSensor<AdcLeftPot, AdcRightPot>::is_calibration_mode_;

template<typename AdcLeftPot, typename AdcRightPot>
bool BumperSensor<AdcLeftPot, AdcRightPot>::last_is_calibration_mode_;

template<typename AdcLeftPot, typename AdcRightPot>
uint8_t BumperSensor<AdcLeftPot, AdcRightPot>::left_;

template<typename AdcLeftPot, typename AdcRightPot>
uint8_t BumperSensor<AdcLeftPot, AdcRightPot>::right_;


#endif
