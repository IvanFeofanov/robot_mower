#ifndef MOTORS_CONTROLLER_CLIENT_H
#define MOTORS_CONTROLLER_CLIENT_H

typedef void (*RpsCallbackFunction)(int16_t, int16_t);
struct RpsCallback
{
    RpsCallbackFunction pntr;
};

typedef void (*OdometerCallbackFunction)(uint32_t, uint32_t);
struct OdometerCallback
{
    OdometerCallbackFunction pntr;
};

template<
    typename Twi,
    uint8_t ADDRESS
    >
class MotorsControllerClient
{
private:
    enum{
        state_rps_sending,
        state_rps_request,
        state_rps_receive,
        state_counter_request,
        state_counter_receive
    };

    enum{
        RPS,
        COUNTER
    };

public:
    static void init()
    {
        state_ = state_rps_sending;

        is_new_set_rps_ = false;
        left_set_rps_ = 0;
        right_set_rps_ = 0;

        rps_callback_.pntr = 0;
        odometer_callback_.pntr = 0;
    }

    static void update()
    {
        if(!Twi::isReady()){
            return;
        }

        int16_t left_rps  = 0;
        int16_t right_rps = 0;
        uint32_t left_counter = 0;
        uint32_t right_counter = 0;

        switch(state_)
        {
        case state_rps_sending:
            if(is_new_set_rps_){
                sendRps(left_set_rps_, right_set_rps_);
                is_new_set_rps_ = false;
            }

            if(rps_callback_.pntr != 0){
                state_ = state_rps_request;
            }else if(odometer_callback_.pntr != 0){
                state_ = state_counter_request;
            }else{
                state_ = state_rps_sending;
            }
            // state_ = state_rps_sending;
            break;

        case state_rps_request:
            //command
            buffer_[0] = RPS;

            Twi::requestFrom(ADDRESS, buffer_, 1, buffer_, 4);
            state_ = state_rps_receive;
            break;

        case state_rps_receive:
            left_rps = (buffer_[0] << 8) | buffer_[1];
            right_rps = (buffer_[2] << 8) | buffer_[3];

            if(rps_callback_.pntr != 0){
                rps_callback_.pntr(left_rps, right_rps);
            }

            if(odometer_callback_.pntr != 0){
                state_ = state_counter_request;
            }else{
                state_ = state_rps_sending;
            }
            break;

        case state_counter_request:
            buffer_[0] = COUNTER;
            Twi::requestFrom(ADDRESS, buffer_, 1, buffer_, 8);
            state_ = state_counter_receive;
            break;

        case state_counter_receive:
            left_counter = (buffer_[0] << 24) | (buffer_[1] << 16)
                          | (buffer_[2] << 8) | buffer_[3];
            right_counter = (buffer_[4] << 24) | (buffer_[5] << 16)
                          | (buffer_[6] << 8) | buffer_[7];

            if(odometer_callback_.pntr != 0){
                odometer_callback_.pntr(left_counter, right_counter);
            }

            state_ = state_rps_sending;
            break;

        default:
            break;
        }
    }

    static void setSpeed(int16_t left, int16_t right)
    {
        if(!Twi::isReady()){
            left_set_rps_ = left;
            right_set_rps_ = right;
            is_new_set_rps_ = true;
        }else{
            sendRps(left, right);
        }
    }

    static void attachSpeedDataChange(RpsCallbackFunction function)
    {
        rps_callback_.pntr = function;
    }

    static void attachOdometerDataChange(OdometerCallbackFunction function)
    {
        odometer_callback_.pntr = function;
    }

private:
    static void sendRps(int16_t left, int16_t right)
    {
        //command
        buffer_[0] = RPS;
        //left rps
        buffer_[1] = (left >> 8);
        buffer_[2] = left;
        //right
        buffer_[3] = (right >> 8);
        buffer_[4] = right;

        Twi::write(ADDRESS, buffer_, 5);
    }

private:
    static uint8_t state_;

    static bool    is_new_set_rps_;
    static int16_t left_set_rps_;
    static int16_t right_set_rps_;

    static RpsCallback rps_callback_;
    static OdometerCallback odometer_callback_;

    static uint8_t buffer_[16];
};

template<typename Twi, uint8_t ADDRESS>
uint8_t MotorsControllerClient<Twi, ADDRESS>::state_;

template<typename Twi, uint8_t ADDRESS>
bool MotorsControllerClient<Twi, ADDRESS>::is_new_set_rps_;

template<typename Twi, uint8_t ADDRESS>
int16_t MotorsControllerClient<Twi, ADDRESS>::left_set_rps_;

template<typename Twi, uint8_t ADDRESS>
int16_t MotorsControllerClient<Twi, ADDRESS>::right_set_rps_;

template<typename Twi, uint8_t ADDRESS>
uint8_t MotorsControllerClient<Twi, ADDRESS>::buffer_[16];

template<typename Twi, uint8_t ADDRESS>
RpsCallback MotorsControllerClient<Twi, ADDRESS>::rps_callback_;

template<typename Twi, uint8_t ADDRESS>
OdometerCallback MotorsControllerClient<Twi, ADDRESS>::odometer_callback_;







#endif
