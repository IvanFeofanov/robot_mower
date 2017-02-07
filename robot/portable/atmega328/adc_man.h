#ifndef ADC_MAN_H
#define ADC_MAN_H

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
}

enum{
    ADC_SAMPLE_RATE_9615,
    ADC_SAMPLE_RATE_19231,
    ADC_SAMPLE_RATE_38462
};

template<
    uint8_t CHANNELS,
    uint8_t SAMPLE_RATE
    >
class AdcMan
{
public:
    static inline void init()
    {
        current_index_ = 0;

        for(int i = 0; i < CHANNELS; i++){
            capture_complate_[i] = false;
        }

        ADMUX |= (1<<REFS0) | (1<<ADLAR); //voltage reference - AVcc

        ADCSRA |= (1<<ADEN)      // ADC enable
               | (1<<ADIE);       // interrupt enable

        //prescaler selection
        if(SAMPLE_RATE == ADC_SAMPLE_RATE_19231)
            ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0); //64
        else if(SAMPLE_RATE == ADC_SAMPLE_RATE_38462)
            ADCSRA |= (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0); //32
        else //9615
            ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //128

        ADCSRA |= (1<<ADSC); //start conv

    }

    static uint8_t getResult(uint8_t index)
    {
        uint8_t value;

        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            value = capture_[index];
            capture_[index] = 0;
            capture_complate_[index] = false;
        }

        return value;
    }

    static bool isCaptureComplate(uint8_t index)
    {
        bool is_complate;

        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            is_complate = capture_complate_[index];
        }

        return is_complate;
    }

public:
    static inline void interruptHandler()
    {
        capture_[current_index_] = ADCH;
        capture_complate_[current_index_] = true;

        if(current_index_ >= CHANNELS-1){
            current_index_ = 0;
        }else{
            current_index_++;
        }

        ADMUX = (ADMUX & 0b11110000) | current_index_;

        ADCSRA |= (1<<ADSC); //start conv
    }


private:
    volatile static uint8_t capture_[CHANNELS];
    volatile static bool capture_complate_[CHANNELS];
    volatile static uint8_t current_index_;
};

template<uint8_t CHANNELS, uint8_t SAMPLE_RATE>
volatile uint8_t AdcMan<CHANNELS, SAMPLE_RATE>::capture_[CHANNELS];

template<uint8_t CHANNELS, uint8_t SAMPLE_RATE>
volatile bool AdcMan<CHANNELS, SAMPLE_RATE>::capture_complate_[CHANNELS];

template<uint8_t CHANNELS, uint8_t SAMPLE_RATE>
volatile uint8_t AdcMan<CHANNELS, SAMPLE_RATE>::current_index_;

typedef AdcMan<2, ADC_SAMPLE_RATE_9615> Adc;

#endif
