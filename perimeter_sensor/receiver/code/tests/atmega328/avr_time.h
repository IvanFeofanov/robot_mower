#ifndef AVR_TIME_H
#define AVR_TIME_H

#include <stdint.h>

void avr_time_init();
void avr_time_overflow_handler();
uint32_t avr_time_now();

#endif
