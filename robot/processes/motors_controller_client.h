#ifndef MOTORS_CONTROLLER_CLIENT_H
#define MOTORS_CONTROLLER_CLIENT_H

template<
    typename SpiMaster,
    typename CePin
    >
class MotorsController
{
public:
    static inline void init()
    {
        left_set_speed_     = 0;
        right_set_speed_    = 0;
        left_real_speed_    = 0;
        right_real_speed_   = 0;
        left_counter_       = 0;
        right_counter_      = 0;

        state_ = ST_TX_SPEED;
    }

    static inline void update()
    {
        if(!SpiMaster::is_available())
            return;

        switch(state_)
        {
        case ST_TX_SPEED:
            buffer_[0] = 0; //CMD and addr
            memcpy(buffer_ + 1, &left_set_speed_, 2);
            memcpy(buffer_ + 3, &right_set_speed_, 2);

            CePin::setLow();
            SpiMaster::transfer(buffer_, buffer_, 5);

            state_ = ST_RX_SPEED;
            break;

        case ST_RX_SPEED:
            CePin::setHigh();

            //TODO check CRC
            if(true){
                memcpy(&left_real_speed_, buffer_, 2);
                memcpy(&right_real_speed_, buffer_+2, 2);
            }

            state_ = ST_CMD_COUNTER;
            break;

        case ST_CMD_COUNTER:
            buffer_[0] = 1; //CMD and addr

            CePin::setLow();
            SpiMaster::transfer(buffer_, buffer_, 9);

            state_ = ST_RX_COUNTER;
            break;

        case ST_RX_COUNTER:
            CePin::setHigh();

            //TODO check CRC
            if(true){
                memcpy(&left_counter_, buffer_, 4);
                memcpy(&right_counter_, buffer_+4, 4);
            }

            state_ = ST_TX_SPEED;
            break;
        }
    }

    static void set_speed(int16_t left, int16_t right)
    {
        left_set_speed_  = left;
        right_set_speed_ = right;
    }

    static void get_speed(int16_t* left, int16_t* right)
    {
        *left  = left_real_speed_;
        *right = right_real_speed_;
    }

    static void get_counter(uint32_t* left, uint32_t* right)
    {
        *left   = left_counter_;
        *right  = right_counter_;
    }

private:
    static uint8_t state_;
    enum{
        ST_TX_SPEED,
        ST_RX_SPEED,
        ST_CMD_COUNTER,
        ST_RX_COUNTER
    };

    static uint8_t buffer_[10];

    static int16_t  left_set_speed_;
    static int16_t  right_set_speed_;
    static int16_t  left_real_speed_;
    static int16_t  right_real_speed_;
    static uint32_t left_counter_;
    static uint32_t right_counter_;
};

template<
    typename SpiMaster,
    typename CePin
    >
uint8_t MotorsController<SpiMaster, CePin>::state_;

template<
    typename SpiMaster,
    typename CePin
    >
uint8_t MotorsController<SpiMaster, CePin>::buffer_[10];

template<
    typename SpiMaster,
    typename CePin
    >
int16_t MotorsController<SpiMaster, CePin>::left_set_speed_;

template<
    typename SpiMaster,
    typename CePin
    >
int16_t MotorsController<SpiMaster, CePin>::right_set_speed_;

template<
    typename SpiMaster,
    typename CePin
    >
int16_t MotorsController<SpiMaster, CePin>::left_real_speed_;

template<
    typename SpiMaster,
    typename CePin
    >
int16_t MotorsController<SpiMaster, CePin>::right_real_speed_;

template<
    typename SpiMaster,
    typename CePin
    >
uint32_t MotorsController<SpiMaster, CePin>::left_counter_;

template<
    typename SpiMaster,
    typename CePin
    >
uint32_t MotorsController<SpiMaster, CePin>::right_counter_;

#endif
