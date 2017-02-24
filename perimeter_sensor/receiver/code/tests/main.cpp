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
    static uint32_t last_time = 0;
    static uint16_t counter = 0;

    if(AdcMan::isCaptureComplate())
    {
        receiver.calculate((int16_t*)AdcMan::capture_value);
        int16_t value = receiver.getSmoothMagnitude();
        counter++;
        sprintf(buffer, "%d\n", value);
        hardware.write(buffer, strlen(buffer));
        AdcMan::start();
    }

    // if(hardware.time() - last_time >= 1000){
    //     sprintf(buffer, "%d\n", counter);
    //     hardware.write(buffer, strlen(buffer));
    //
    //     counter = 0;
    //     last_time = hardware.time();
    // }

}


int main()
{
    setup();
    while(true)
        loop();

    return 0;
}
