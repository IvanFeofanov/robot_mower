#ifndef DRIVE_MOTORS
#define DRIVE_MOTORS

template<
    typename Twi,
    uint8_t ADDRESS
    >
class DriveMotors
{
private:
    enum{
        RPS_SENDING,
        RPS_REQUEST,
        RECEIVE_RPS,
        COUNTER_REQUEST,
        RECEIVE_COUNTER
    };

    enum{
        TYPE_RPS,
        TYPE_COUNTER
    };

public:
    void init()
    {
        state_ = RPS_SENDING;
    }

    void run(int16_t left_set_speed, int16_t right_set_speed,
            int16_t* left_real_speed, int16_t* right_real_speed,
            uint32_t* left_counter, uint32_t* right_counter)
    {
        if(!Twi::isReady()){
            return;
        }

        switch(state_)
        {
        case RPS_SENDING:
            //type
            buffer_[0] = TYPE_RPS;
            //left speed
            buffer_[1] = (left_set_speed >> 8);
            buffer_[2] = left_set_speed;
            //right speed
            buffer_[3] = (right_set_speed >> 8);
            buffer_[4] = right_set_speed;

            Twi::write(ADDRESS, buffer_, 5);

            state_ = RPS_REQUEST;
            break;

        case RPS_REQUEST:
            buffer_[0] = TYPE_RPS; //type
            Twi::requestFrom(ADDRESS, buffer_, 1, buffer_, 4);
            state_ = RECEIVE_RPS;
            break;

        case RECEIVE_RPS:
            *left_real_speed = (buffer_[0] << 8) | buffer_[1];
            *right_real_speed = (buffer_[2] << 8) | buffer_[3];

            state_ = COUNTER_REQUEST;
            break;

        case COUNTER_REQUEST:
            buffer_[0] = TYPE_COUNTER;
            Twi::requestFrom(ADDRESS, buffer_, 1, buffer_, 8);
            state_ = RECEIVE_COUNTER;
            break;

        case RECEIVE_COUNTER:
            *left_counter = (buffer_[0] << 24) | (buffer_[1] << 16)
                          | (buffer_[2] << 8) | buffer_[3];
            *right_counter = (buffer_[4] << 24) | (buffer_[5] << 16)
                          | (buffer_[6] << 8) | buffer_[7];

            state_ = RPS_SENDING;
            break;

        default:
            break;
        }
    }

private:
    uint8_t state_;

    uint8_t buffer_[16];
};

#endif
