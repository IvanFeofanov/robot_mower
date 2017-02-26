#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct MowerMsg
{
    uint8_t speed;
    bool is_enable;
}MowerMsg;

typedef class DriveMotorsMsg
{
public:
    void setSpeed(int16_t left_speed, int16_t right_speed)
    {
        left_set_speed = left_speed;
        right_set_speed = right_speed;
        is_new_data = true;
    }

    bool is_new_data;
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
    bool is_save;
}BumperMsg;

typedef struct PerimeterSensorMsg
{
    int16_t signal_offset;

    uint16_t lps;

    int16_t magnitude;
    int16_t smooth_magnitude;
    bool is_inside;
    bool is_new_data;

    bool is_invert_sigcode;
    bool is_offset_calibration_mode;

}PerimeterSensorMsg;

#endif
