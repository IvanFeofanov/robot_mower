#ifndef CRC8_H
#define CRC8_H

#include <stdint.h>

uint8_t crc8_one_byte(uint8_t byte, uint8_t prev_value);
uint8_t calc_crc8(const uint8_t* data, uint8_t len);

#endif
