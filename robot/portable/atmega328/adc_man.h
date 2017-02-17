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

struct AdcSlot
{
    uint16_t* capture_buffer;
    uint16_t capture_size;
    uint8_t channel;
    bool is_complate;
};

template<
    uint8_t SLOTS,
    uint8_t SAMPLE_RATE
    >
class AdcMan
{
public:
    enum{
        ADC0 = 0x00,
        ADC1 = 0x01,
        ADC2 = 0x02,
        ADC3 = 0x03
    };

    enum{
        ADC_SAMPLE_RATE = SAMPLE_RATE
    };

    static inline void init()
    {
        ADMUX |= (1<<REFS0); //voltage reference - AVcc

        ADCSRA |= (1<<ADIE)     //ADC interrupt enable
               | (1<<ADATE);    //ADC auto trigger enable

        //prescaler selection (for F_CPU = 16Mhz)
        if(SAMPLE_RATE == ADC_SAMPLE_RATE_19231)
            ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0); //64
        else if(SAMPLE_RATE == ADC_SAMPLE_RATE_38462)
            ADCSRA |= (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0); //32
        else //9615
            ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //128

        //set default value
        for(int i = 0; i < SLOTS; i++){
            slots_[i].is_complate = true;
            slots_[i].channel = ADC0;
            slots_[i].capture_size = 0;
        }

        capture_index_ = 0;
        current_slot_ = 0;

        setChannel(ADC0);
        startConv();

    }

    static void setSlot(uint8_t slot, uint8_t channel,
            uint16_t* capture_buffer, uint8_t capture_size)
    {
        slots_[slot].channel = channel;
        slots_[slot].capture_buffer = capture_buffer;
        slots_[slot].capture_size = capture_size;
    }

    static void startCapture(uint8_t slot)
    {
        slots_[slot].is_complate = false;
    }

    static uint16_t* getResult(uint8_t slot)
    {
        return slots_[slot].capture_buffer;
    }

    static bool isCaptureComplate(uint8_t slot)
    {
        bool is_complate;

        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            is_complate = slots_[slot].is_complate;
        }

        return is_complate;
    }

    static inline void interruptHandler()
    {
        if(slots_[current_slot_].is_complate ||
                capture_index_ >= slots_[current_slot_].capture_size){
            slots_[current_slot_].is_complate = true;
            capture_index_ = 0;
            stopConv();
            nextSlot();
            startConv();
            return;
        }

        slots_[current_slot_].capture_buffer[capture_index_] = ADC;
        capture_index_++;
    }

private:
    static inline void startConv()
    {
        ADCSRA |= (1<<ADEN);
        ADCSRA |= (1<<ADSC);
    }

    static inline void stopConv()
    {
        ADCSRA &= ~(1<<ADEN);
    }

    static inline void setChannel(uint8_t channel)
    {
        ADMUX = (ADMUX & 0b11110000) | channel;
    }

    static inline void nextSlot()
    {
        if(current_slot_ < SLOTS - 1){
            current_slot_++;
        }else{
            current_slot_ = 0;
        }

        setChannel(slots_[current_slot_].channel);
    }

private:
    volatile static AdcSlot slots_[SLOTS];
    volatile static uint8_t current_slot_;
    volatile static uint16_t capture_index_;
};

template<uint8_t SLOTS, uint8_t SAMPLE_RATE>
volatile AdcSlot AdcMan<SLOTS, SAMPLE_RATE>::slots_[SLOTS];

template<uint8_t SLOTS, uint8_t SAMPLE_RATE>
volatile uint8_t AdcMan<SLOTS, SAMPLE_RATE>::current_slot_;

template<uint8_t SLOTS, uint8_t SAMPLE_RATE>
volatile uint16_t AdcMan<SLOTS, SAMPLE_RATE>::capture_index_;


template<
    typename AdcMan,
    uint8_t CHANNEL,
    uint8_t SLOT
    >
class AdcChannel
{
public:
    typedef AdcMan Man;

    static inline void setSlot(uint16_t* capture_buffer, uint8_t capture_size)
    {
        AdcMan::setSlot(SLOT, CHANNEL, capture_buffer, capture_size);
    }

    static inline void startCapture()
    {
        AdcMan::startCapture(SLOT);
    }

    static inline uint16_t* getResult()
    {
        return AdcMan::getResult(SLOT);
    }

    static inline bool isCaptureComplate()
    {
        return AdcMan::isCaptureComplate(SLOT);
    }

};

typedef AdcMan<3, ADC_SAMPLE_RATE_19231> Adc;

typedef AdcChannel<Adc, Adc::ADC0, 0> Adc0;
typedef AdcChannel<Adc, Adc::ADC1, 1> Adc1;
typedef AdcChannel<Adc, Adc::ADC2, 2> Adc2;
typedef AdcChannel<Adc, Adc::ADC3, 3> Adc3;

#endif
