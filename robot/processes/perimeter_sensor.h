#ifndef PERIMETER_SENSOR_H
#define PERIMETER_SENSOR_H

#include "correlation_filter.h"

const signed char sigcode[] =
        { 1,0,-1, 0,1,-1,1,-1, 0,1,-1,1,0,-1, 0,1,-1, 0,1,-1, 0,1,0,-1 };

template<
    typename Adc
    >
class PerimeterSensor
{
public:
    static void init()
    {
        Adc::setSlot((uint16_t*)signal_, SIGNAL_SIZE);
        Adc::startCapture();

        signal_offset_ = 512;
        is_invert_sigcode_ = false;

        magnitude_ = 0;
        is_inside_ = false;

        is_offset_calibration_mode_ = false;
        state_ = state_set_offset;

        // last_time_ = 0;
        // loops_counter_ = 0;
    }

    static void update()
    {
        // //speed test
        // if(Time::now() - last_time_ >= 1000){
        //     msg->lps = loops_counter_;
        //     loops_counter_ = 0;
        //     last_time_ = Time::now();
        // }

        if(!Adc::isCaptureComplate()){
            return;
        }

        //calibration mode
        if(is_offset_calibration_mode_){
            uint16_t max_value = signal_[0];
            uint16_t min_value = signal_[0];

            for(int i = 1; i < SIGNAL_SIZE; ++i)
            {
                if(signal_[i] > max_value)
                    max_value = signal_[i];
                if(signal_[i] < min_value)
                    min_value = signal_[i];
            }

            int16_t center = (max_value - min_value) / 2;
            signal_offset_ = min_value + center;

            is_offset_calibration_mode_ = false;

        }else{ //normal mode

            switch(state_)
            {
            case state_set_offset:
                for(uint8_t i = 0; i < SIGNAL_SIZE; i++){
                    signal_[i] -= signal_offset_;
                }
                state_ = state_calculate;
                break;

            case state_calculate:
                magnitude_ = correlationFilter( signal_,
                                                SIGNAL_SIZE,
                                                sigcode,
                                                sizeof(sigcode),
                                                SUBSAMPLES);
                if(is_invert_sigcode_){
                    magnitude_ *= -1;
                }

                is_inside_ = magnitude_ >= 0;

                state_ = state_set_offset;
                Adc::startCapture();

                // loops_counter_++;
                break;
            }
        }
    }

    static inline void setIsInvertSigcode(bool is_invert)
    {
        is_invert_sigcode_ = is_invert;
    }

    static inline bool isInvertSigcode()
    {
        return is_invert_sigcode_;
    }

    static inline int16_t magnitude()
    {
        return magnitude_;
    }

    static inline bool isInside()
    {
        return is_inside_;
    }

    static inline void calibrate()
    {
        is_offset_calibration_mode_ = true;
    }

private:

    enum { SENDER_RATE = 9615 };
    enum { SAMPLE_RATE = 19231}; // = sample rate of adc
    enum { SUBSAMPLES = SAMPLE_RATE / SENDER_RATE };
    enum { SIGNAL_SIZE = sizeof(sigcode) * 2 * SUBSAMPLES };

    static int16_t signal_offset_;
    static bool is_invert_sigcode_;

    static int16_t signal_[SIGNAL_SIZE];
    static int16_t magnitude_;
    static bool is_inside_;

    static bool is_offset_calibration_mode_;
    static uint8_t state_;

    enum{
        state_set_offset,
        state_calculate,
    };

    // static uint32_t last_time_;
    // static uint16_t loops_counter_;
    // static uint16_t lps;
};

template<typename Adc>
int16_t PerimeterSensor<Adc>::signal_offset_;

template<typename Adc>
bool PerimeterSensor<Adc>::is_invert_sigcode_;

template<typename Adc>
int16_t PerimeterSensor<Adc>::signal_[SIGNAL_SIZE];

template<typename Adc>
int16_t PerimeterSensor<Adc>::magnitude_;

template<typename Adc>
bool PerimeterSensor<Adc>::is_inside_;

template<typename Adc>
bool PerimeterSensor<Adc>::is_offset_calibration_mode_;

template<typename Adc>
uint8_t PerimeterSensor<Adc>::state_;


#endif
