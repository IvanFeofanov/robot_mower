#ifndef ADC_MAN_H
#define ADC_MAN_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// #define SAMPLE_RATE 9615
// #define SAMPLE_RATE 19231
#define SAMPLE_RATE 38462

#define SUBSAMPLES (SAMPLE_RATE / 9615)
#define CAPTURE_SIZE (24 * 2 * SUBSAMPLES)


class AdcMan
{
public:
    static void init();
    static void start();
    static void adc_init();
    static void adcCallback();
    static bool isCaptureComplate();

    volatile static int16_t capture_value[CAPTURE_SIZE];
    volatile static uint16_t capture_counter;
    volatile static bool     is_capture_complate;
};



#endif
