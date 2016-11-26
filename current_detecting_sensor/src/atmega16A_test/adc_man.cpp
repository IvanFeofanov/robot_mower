
#include "adc_man.h"

volatile int16_t AdcMan::capture_value[CAPTURE_SIZE];
volatile uint16_t AdcMan::capture_counter;
volatile bool AdcMan::is_capture_complate;


ISR(ADC_vect)
{
    AdcMan::adcCallback();
}

void AdcMan::init(Mode mode)
{
    adc_init(mode);

    capture_counter = 0;
    is_capture_complate  = false;

}

void AdcMan::start()
{
    is_capture_complate = false;
}

void AdcMan::adc_init(Mode mode)
{
    ADMUX |= (1<<REFS0); //voltage reference - AVcc

    if(mode == NEGATIVE_INPUT)
        ADMUX &= ~((1<<MUX4) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
    else if(mode == POSITIVE_INPUT)
        ADMUX |= (1<<MUX0);
    else if(mode == DIFF_10X)
        ADMUX |= (0<<MUX4) | (1<<MUX3) | (0<<MUX2) | (0<<MUX1) | (1<<MUX0);
    else //DIFF_20X
        ADMUX |= (0<<MUX4) | (1<<MUX3) | (0<<MUX2) | (1<<MUX1) | (1<<MUX0);


    ADCSRA |= (1<<ADEN)      // ADC enable
           | (1<<ADIE)       // interrupt enable
           | (1<<ADATE);     // auto trigger Enable

    //prescaler selection
    if(SAMPLE_RATE == 19231)
        ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0); //64
    else if(SAMPLE_RATE == 38462)
        ADCSRA |= (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0); //32
    else //9615
        ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //128

    ADCSRA |= (1<<ADSC); //start conv

}

void AdcMan::adcCallback()
{
    if(!is_capture_complate)
    {
        uint16_t res = ADC;
        capture_value[capture_counter] = (ADCH & (1<<1)) ? -res : res;

        if(capture_counter + 1 < CAPTURE_SIZE){
            capture_counter++;
        }else{
            capture_counter = 0;
            is_capture_complate = true;
        }
    }
}

bool AdcMan::isCaptureComplate()
{
    return is_capture_complate;
}


