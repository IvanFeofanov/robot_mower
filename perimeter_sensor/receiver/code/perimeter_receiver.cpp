#include "perimeter_receiver.h"
#include <avr/io.h>

PerimeterReceiver::PerimeterReceiver()
{
    offset_             = 512; //2^10 / 2
    magnitude_          = 0;
    smooth_magnitude_   = 0;
    is_inside_          = false;
    is_sender_enable_   = false;
    intersections_      = 0;
    is_invert_signal_   = false;
}

void PerimeterReceiver::calculate(const int16_t* input_array)
{
    magnitude_ = correlationFilter( input_array,
                                    PR_INPUT_BUFFER_SIZE,
                                    sigcode,
                                    sizeof(sigcode),
                                    PR_SUBSAMPLES);
    if(is_invert_signal_)
        magnitude_ *= -1;

    smooth_magnitude_ = 0.9 * (float)(smooth_magnitude_)
        + 0.1 * (float)(magnitude_);
}

void PerimeterReceiver::setOffset(uint16_t offset)
{
    offset_ = offset;
}

void PerimeterReceiver::calculateOffset(const int16_t* input_array)
{
    int16_t max_value = input_array[0];
    int16_t min_value = max_value;

    for(int i = 1; i < PR_INPUT_BUFFER_SIZE; ++i)
    {
        if(input_array[i] > max_value)
            max_value = input_array[i];
        if(input_array[i] < min_value)
            min_value = input_array[i];
    }

    int16_t center = (max_value - min_value) / 2;
    offset_ = min_value + center;
}

void PerimeterReceiver::setInvertSignal(bool is_invert)
{
    is_invert_signal_ = is_invert;
}

int16_t PerimeterReceiver::getMagnitude() const
{
    return magnitude_;
}

int16_t PerimeterReceiver::getSmoothMagnitude() const
{
    return smooth_magnitude_;
}

bool PerimeterReceiver::isInside() const
{
    return is_inside_;
}

bool PerimeterReceiver::isSenderEnable() const
{
    return is_sender_enable_;
}

uint16_t PerimeterReceiver::getIntersections() const
{
    return intersections_;
}

bool PerimeterReceiver::isInvertSignal() const
{
    return  is_invert_signal_;
}

int32_t PerimeterReceiver::correlationFilter(   const int16_t* input_array,
                                                uint16_t input_array_size,
                                                const int8_t*  sigcode,
                                                int16_t  sigcode_size,
                                                int8_t   n_subsamples)

{
    int32_t max_value = 0;
    int32_t min_value = 0;

    for(uint16_t shift = 0; shift < input_array_size - sigcode_size * n_subsamples; shift++)
    {
        int32_t sum_val = 0;
        uint8_t subsample = 0;
        uint8_t sc = 0;
        for(int i = 0; i < sigcode_size * n_subsamples; i++){
            sum_val += (input_array[shift + i] - offset_) * sigcode[sc];
            subsample++;
            if(subsample == n_subsamples){
                sc++;
                subsample = 0;
            }
        }

        if(sum_val > max_value)
            max_value = sum_val;
        if(sum_val < min_value)
            min_value = sum_val;
    }

    if(max_value > -min_value){
        return max_value;
    }else{
        return min_value;
    }
}
