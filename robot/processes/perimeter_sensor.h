#ifndef PERIMETER_SENSOR_H
#define PERIMETER_SENSOR_H

#include "types.h"
#include "correlation_filter.h"

const signed char sigcode[] =
        { 1,0,-1, 0,1,-1,1,-1, 0,1,-1,1,0,-1, 0,1,-1, 0,1,-1, 0,1,0,-1 };

template<
    typename Time,
    typename Adc
    >
class PerimeterSensor
{
public:
    void init()
    {
        Adc::setSlot((uint16_t*)signal_, SIGNAL_SIZE);
        Adc::startCapture();

        state_ = state_set_offset;
        last_time_ = 0;
        loops_counter_ = 0;
    }

    void run(PerimeterSensorMsg* msg)
    {
        //speed test
        if(Time::now() - last_time_ >= 1000){
            msg->lps = loops_counter_;
            loops_counter_ = 0;
            last_time_ = Time::now();
        }

        if(!Adc::isCaptureComplate()){
            return;
        }

        //calibration mode
        if(msg->is_offset_calibration_mode){
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
            msg->signal_offset = min_value + center;

            msg->is_offset_calibration_mode = false;

        }else{ //normal mode

            switch(state_)
            {
            case state_set_offset:
                for(uint8_t i = 0; i < SIGNAL_SIZE; i++){
                    signal_[i] -= msg->signal_offset;
                }
                state_ = state_calculate;
                break;

            case state_calculate:
                msg->magnitude = correlationFilter( signal_,
                                                    SIGNAL_SIZE,
                                                    sigcode,
                                                    sizeof(sigcode),
                                                    SUBSAMPLES);
                if(msg->is_invert_sigcode){
                    msg->magnitude *= -1;
                }

                msg->is_inside = msg->magnitude >= 0;
                msg->is_new_data = true;

                loops_counter_++;

                state_ = state_set_offset;
                Adc::startCapture();
                break;
            }
        }

    }

private:

    enum { SENDER_RATE = 9615 };
    enum { SAMPLE_RATE = 19231}; // = sample rate of adc
    enum { SUBSAMPLES = SAMPLE_RATE / SENDER_RATE };
    enum { SIGNAL_SIZE = sizeof(sigcode) * 2 * SUBSAMPLES };

    int16_t signal_[SIGNAL_SIZE];
    uint8_t state_;

    enum{
        state_set_offset,
        state_calculate,
    };

    uint32_t last_time_;
    uint16_t loops_counter_;
};

#endif
