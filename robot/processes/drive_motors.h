#ifndef DRIVE_MOTORS
#define DRIVE_MOTORS

#include "types.h"

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

    void run(DriveMotorsMsg* drive_motors_msg)
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
            buffer_[1] = (drive_motors_msg->left_set_speed >> 8);
            buffer_[2] = drive_motors_msg->left_set_speed;
            //right speed
            buffer_[3] = (drive_motors_msg->right_set_speed >> 8);
            buffer_[4] = drive_motors_msg->right_set_speed;

            Twi::write(ADDRESS, buffer_, 5);

            state_ = RPS_REQUEST;
            break;

        case RPS_REQUEST:
            buffer_[0] = TYPE_RPS; //type
            Twi::requestFrom(ADDRESS, buffer_, 1, buffer_, 4);
            state_ = RECEIVE_RPS;
            break;

        case RECEIVE_RPS:
            drive_motors_msg->left_real_speed = (buffer_[0] << 8) | buffer_[1];
            drive_motors_msg->right_real_speed = (buffer_[2] << 8) | buffer_[3];

            state_ = COUNTER_REQUEST;
            break;

        case COUNTER_REQUEST:
            buffer_[0] = TYPE_COUNTER;
            Twi::requestFrom(ADDRESS, buffer_, 1, buffer_, 8);
            state_ = RECEIVE_COUNTER;
            break;

        case RECEIVE_COUNTER:
            drive_motors_msg->left_counter = (buffer_[0] << 24) | (buffer_[1] << 16)
                          | (buffer_[2] << 8) | buffer_[3];
            drive_motors_msg->right_counter = (buffer_[4] << 24) | (buffer_[5] << 16)
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
