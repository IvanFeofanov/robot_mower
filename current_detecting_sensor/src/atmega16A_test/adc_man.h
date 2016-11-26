#ifndef ADC_MAN_H
#define ADC_MAN_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// #define SAMPLE_RATE 9615
// #define SAMPLE_RATE 19231
#define SAMPLE_RATE 38462

#define CAPTURE_SIZE 100

class AdcMan
{
public:
    enum Mode
    {
        NEGATIVE_INPUT,
        POSITIVE_INPUT,
        DIFF_10X,
        DIFF_200X
    };

    static void init(Mode mode);
    static void start();
    static void adc_init(Mode mode);
    static void adcCallback();
    static bool isCaptureComplate();

    volatile static int16_t capture_value[CAPTURE_SIZE];
    volatile static uint16_t capture_counter;
    volatile static bool     is_capture_complate;

};



#endif
