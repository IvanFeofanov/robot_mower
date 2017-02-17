#include "correlation_filter.h"

int32_t correlationFilter(const int16_t* input_array,
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
            sum_val += (input_array[shift + i]) * sigcode[sc];
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

