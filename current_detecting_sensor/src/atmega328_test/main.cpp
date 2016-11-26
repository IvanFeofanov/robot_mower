extern "C"
{
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
}

#include "adc_man.h"
#include "atmega328/Atmega328Hardware.h"
// #include "./../perimeter_receiver.h"

Atmega328Hardware hardware;

void setup()
{
    AdcMan::init();
    hardware = Atmega328Hardware();
    hardware.init();

}

void loop()
{
    char buffer[64];

    if(AdcMan::isCaptureComplate())
    {
        for(int i = 0; i < CAPTURE_SIZE; i++){
            sprintf(buffer, "%d\n", AdcMan::capture_value[i]);
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
