#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H

template<
    typename LedPin,
    typename Time
    >
class LedIndiactor
{
public:
    void init()
    {
        LedPin::setOutput();
        LedPin::setLow();
    }

    void run(const uint8_t* status)
    {
        // static uint32_t last_time = 0;
        // if(Time::now() - last_time > 500){
        //     LedPin::toggle();
        //     last_time = Time::now();
        // }
        if(*status){
            LedPin::setHigh();
        }else{
            LedPin::setLow();
        }
    }

private:

};


#endif
