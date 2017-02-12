#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct MowerMsg
{
    uint8_t speed;
    bool is_enable;
}MowerMsg;

typedef struct DriveMotorsMsg
{
    int16_t left_set_speed;
    int16_t right_set_speed;
    int16_t left_real_speed;
    int16_t right_real_speed;
    uint32_t left_counter;
    uint32_t right_counter;
}DriveMotorsMsg;

#endif
