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
#include "./../perimeter_receiver.h"

Atmega328Hardware hardware;
PerimeterReceiver receiver;

void setup()
{
    AdcMan::init();
    hardware = Atmega328Hardware();
    hardware.init();
    receiver = PerimeterReceiver();

    //set offset
    while(!AdcMan::isCaptureComplate());
    receiver.calculateOffset((int16_t*)AdcMan::capture_value);
    AdcMan::start();

}

void loop()
{
    char buffer[64];

    if(AdcMan::isCaptureComplate())
    {
        receiver.calculate((int16_t*)AdcMan::capture_value);

        sprintf(buffer, "%d\n", receiver.getMagnitude());
        hardware.write(buffer, strlen(buffer));

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
