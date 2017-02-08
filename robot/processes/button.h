#ifndef BUTTON_H
#define BUTTON_H

template<
    typename ButtonPin,
    typename Time
    >
class Button
{
public:
    void init()
    {
        ButtonPin::setInput();
    }

    void run(uint8_t* status)
    {
        if(ButtonPin::isHigh())
        {
            *status = 1;
        }else{
            *status = 0;
        }
    }
};

#endif
