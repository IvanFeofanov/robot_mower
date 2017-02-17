#ifndef CORRELATION_FILTER_H
#define CORRELATION_FILTER_H

#include <stdint.h>

int32_t correlationFilter(const int16_t* input_array,
                          uint16_t input_array_size,
                          const int8_t*  sigcode,
                          int16_t  sigcode_size,
                          int8_t   n_subsamples);


#endif
