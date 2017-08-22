#include "crc8.h"

uint8_t crc8_one_byte(uint8_t byte, uint8_t prev_value)
{
    uint8_t crc = prev_value ^ byte;

    for(uint8_t i = 0; i < 8; i++){
       crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }

    return crc;
}

uint8_t calc_crc8(const uint8_t* data, uint8_t len)
{
    uint8_t crc = 0xff;

    for(uint8_t i = 0; i < len; i++){
        crc = crc8_one_byte(data[i], crc);
    }

    return crc;
}


