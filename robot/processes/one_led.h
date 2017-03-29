#ifndef ONE_LED_H
#define ONE_LED_H

template<
    typename LedPin
    >
class OneLed
{
public:
    static void init()
    {
        LedPin::setOutput();
        LedPin::setLow();

        blink_period_ = 1000; //ms
        blink_impulse_len_ = 500; //ms
        blink_start_impulse_ = 0;

        is_blink_ = false;
    }

    static void update()
    {
        if(is_blink_){
            if(Time::now() - blink_start_impulse_ < blink_impulse_len_){
                LedPin::setHigh();
            }else{
                LedPin::setLow();
            }

            if(Time::now() - blink_start_impulse_ >= blink_period_){
                blink_start_impulse_ = Time::now();
            }
        }

    }

    static void setBlink(uint16_t blink_period, uint16_t blink_impulse_len)
    {
        blink_period_ = blink_period;
        blink_impulse_len_ = blink_impulse_len_;
    }

    static void blink()
    {
        is_blink_ = true;
        blink_start_impulse_ = 0;
    }

    static void setHigh()
    {
        is_blink_ = false;
        LedPin::setHigh();
    }

    static void setLow()
    {
        is_blink_ = false;
        LedPin::setLow();
    }


private:
    static bool is_blink_;
    static uint16_t blink_period_;
    static uint16_t blink_impulse_len_;
    static uint32_t blink_start_impulse_;
};

template<typename LedPin>
bool OneLed<LedPin>::is_blink_;

template<typename LedPin>
uint16_t OneLed<LedPin>::blink_period_;

template<typename LedPin>
uint16_t OneLed<LedPin>::blink_impulse_len_;

template<typename LedPin>
uint32_t OneLed<LedPin>::blink_start_impulse_;





#endif
