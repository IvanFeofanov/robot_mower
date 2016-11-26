
/*
 * Пограмма производит измерение напряжение на выходе датчика Холла 
 * используя диффиренцированные каналы ADC и оправляет результат по UART
 */

extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "adc_man.h"
#include "atmega16A/Atmega16AHardware.h"

#define SMOOTH_RESULT

Atmega16AHardware hardware;
volatile int16_t result;

void setup()
{
    AdcMan::init(AdcMan::DIFF_200X);
    // AdcMan::init(AdcMan::POSITIVE_INPUT);
    // AdcMan::init(AdcMan::NEGATIVE_INPUT);
    hardware = Atmega16AHardware();
    hardware.init();
}

void loop()
{
    char buffer[64];

    if(AdcMan::isCaptureComplate())
    {
        for(int i = 0; i < CAPTURE_SIZE; i++){
#ifdef SMOOTH_RESULT
            result = (float)(result) * 0.99 +
                     (float)(AdcMan::capture_value[i]) * 0.01;
#else
            result = AdcMan::capture_value[i];
#endif
            sprintf(buffer, "%d\n", result);
            hardware.write(buffer, strlen(buffer));
        }

        AdcMan::start();
    }

}


int main()
{
    setup();
    while(true)
        loop();

    return 0;
}
