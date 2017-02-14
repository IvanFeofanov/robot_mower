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

typedef struct BumperMsg
{
    uint8_t left;
    uint16_t left_min;
    uint16_t left_max;

    uint8_t right;
    uint16_t right_min;
    uint16_t right_max;

    bool is_calibration;
}BumperMsg;

#endif
