#include "map.h"

uint16_t map(uint16_t src, uint8_t src_min, uint8_t src_max,
        uint8_t dst_min, uint8_t dst_max)
{
    if(src < src_min)
        return dst_min;
    if(src > src_max)
        return dst_max;
    uint16_t src_delta = src_max - src_min;
    uint16_t dst_delta = dst_max - dst_min;

    if(src_delta == 0)
        return dst_max;

    return dst_min + dst_delta*(src-src_min)/src_delta;
}

