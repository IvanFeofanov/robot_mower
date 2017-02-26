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
        if(*status){
            // LedPin::setHigh();
        }else{
            // LedPin::setLow();
        }
    }

private:

};


#endif
