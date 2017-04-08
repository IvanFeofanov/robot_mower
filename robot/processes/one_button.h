#ifndef ONE_BUTTON_H
#define ONE_BUTTON_H

typedef void (*ClickCallbackFunction)(void);

struct ClickCallback
{
    ClickCallbackFunction pntr;
};

template<
    typename ButtonPin,
    bool ACTIVE_VALUE
    >
class OneButton
{
public:
    static void init()
    {
        ButtonPin::setInput();

        click_callback_.pntr = 0;
        last_time_ = 0;
        click_confirmation_time_ = 100; //ms
        state_ = state_waiting_of_click;
    }

    static void update()
    {
        switch(state_)
        {
        case state_waiting_of_click:
            if(isButtonPressed()){
                state_ = state_waiting_for_confirmation;
                last_time_ = Time::now();
            }
            break;

        case state_waiting_for_confirmation:
            if(isButtonPressed()){
                if(Time::now() - last_time_ >= click_confirmation_time_){
                    state_ = state_waiting_for_release;
                }
            }else{
                state_ = state_waiting_of_click;
            }
            break;

        case state_waiting_for_release:
            if(!isButtonPressed()){
                if(click_callback_.pntr != 0){
                    click_callback_.pntr();
                }
                state_ = state_waiting_of_click;
            }
            break;

        default:
            break;
        }
    }

    static void attachClick(ClickCallbackFunction function)
    {
        click_callback_.pntr = function;
    }


    static void detachClick()
    {
        click_callback_.pntr = 0;
    }

private:
    static inline bool isButtonPressed()
    {
        return ButtonPin::isHigh() == ACTIVE_VALUE;
    }

private:
    static ClickCallback click_callback_;

    static uint32_t last_time_;
    static uint32_t click_confirmation_time_;

    enum{
        state_waiting_of_click,
        state_waiting_for_confirmation,
        state_waiting_for_release,
    };
    static uint8_t state_;

};

template<typename ButtonPin, bool ACTIVE_VALUE>
ClickCallback OneButton<ButtonPin, ACTIVE_VALUE>::click_callback_;

template<typename ButtonPin, bool ACTIVE_VALUE>
uint32_t OneButton<ButtonPin, ACTIVE_VALUE>::last_time_;

template<typename ButtonPin, bool ACTIVE_VALUE>
uint32_t OneButton<ButtonPin, ACTIVE_VALUE>::click_confirmation_time_;

template<typename ButtonPin, bool ACTIVE_VALUE>
uint8_t OneButton<ButtonPin, ACTIVE_VALUE>::state_;


#endif
