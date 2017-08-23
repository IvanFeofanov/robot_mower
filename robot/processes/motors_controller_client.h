#ifndef MOTORS_CONTROLLER_CLIENT_H
#define MOTORS_CONTROLLER_CLIENT_H

#include "utils/crc8.h"
#include "spi_face_defs.h"

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
        len_ = 0;
        last_time_ = 0;
    }

    static inline void update()
    {
        if(!transfer_update())
            return;

        uint8_t is_valid_data = true;

        switch(state_)
        {
        case ST_TX_SPEED:
            buffer_[0] = CMD_WR_REG | (REG_MASK & REG_LEFT_SPEED);
            memcpy(buffer_ + 1, &left_set_speed_, 2);
            buffer_[4] = CMD_WR_REG | (REG_MASK & REG_RIGHT_SPEED);
            memcpy(buffer_ + 5, &right_set_speed_, 2);

            CePin::setLow();
            transfer(8);

            state_ = ST_RX_SPEED;
            break;

        case ST_RX_SPEED:
            CePin::setHigh();

            if(calc_crc8(buffer_+1, 2) == buffer_[3]){
                memcpy(&left_real_speed_, buffer_+1, 2);
            }else{
                is_valid_data = false;
            }

            if(calc_crc8(buffer_+5, 2) == buffer_[7]){
                memcpy(&right_real_speed_, buffer_+5, 2);
            }else{
                is_valid_data = false;
            }

            if(is_valid_data){
                state_ = ST_CMD_COUNTER;
            }else{
                state_ = ST_TX_SPEED;
            }
            break;

        case ST_CMD_COUNTER:
            buffer_[0] = CMD_R_REG | (REG_MASK & REG_LEFT_COUNTER);
            buffer_[6] = CMD_R_REG | (REG_MASK & REG_RIGHT_COUNTER);

            CePin::setLow();
            transfer(12);

            state_ = ST_RX_COUNTER;
            break;

        case ST_RX_COUNTER:
            CePin::setHigh();

            if(calc_crc8(buffer_+1, 4) == buffer_[5]){
                memcpy(&left_counter_, buffer_+1, 4);
            }else{
                is_valid_data = false;
            }

            if(calc_crc8(buffer_+7, 4) == buffer_[11]){
                memcpy(&right_counter_, buffer_+7, 4);
            }else{
                is_valid_data = false;
            }

            if(is_valid_data){
                state_ = ST_TX_SPEED;
            }else{
                state_ = ST_RX_COUNTER;
            }
            break;
        }
    }

    static void set_left_speed(int16_t left)
    {
        left_set_speed_ = left;
    }

    static void set_right_speed(int16_t right)
    {
        right_set_speed_ = right;
    }

    static int16_t real_left_speed()
    {
        return left_real_speed_;
    }

    static int16_t real_right_speed()
    {
        return right_real_speed_;
    }

    static uint32_t left_counter()
    {
        return left_counter_;
    }

    static uint32_t right_counter()
    {
        return right_counter_;
    }

private:
    static void transfer(uint16_t len)
    {
        if(len > 0){
            SpiMaster::transfer(buffer_, buffer_, 1);
            last_time_ = Time::now();
        }

        if(len > 1){
            len_ = len;
            index_ = 1;
        }else{
            len_ = 0;
        }

    }

    static bool transfer_update()
    {
        if(SpiMaster::is_available() && ((Time::now() - last_time_) >= 1)){
            if(len_ != 0){
                SpiMaster::transfer(buffer_+index_, buffer_+index_, 1);

                index_++;
                last_time_ = Time::now();

                if(index_ >= len_){
                    len_ = 0;
                }
                return false;
            }else{
                return true;
            }
        }else{
            return false;
        }
    }

private:
    static uint8_t state_;
    enum{
        ST_TX_SPEED,
        ST_RX_SPEED,
        ST_CMD_COUNTER,
        ST_RX_COUNTER
    };

    static uint8_t buffer_[16];
    static uint8_t index_;
    static uint8_t len_;
    static uint32_t last_time_;

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
uint8_t MotorsController<SpiMaster, CePin>::buffer_[16];

template<
    typename SpiMaster,
    typename CePin
    >
uint8_t MotorsController<SpiMaster, CePin>::index_;

template<
    typename SpiMaster,
    typename CePin
    >
uint8_t MotorsController<SpiMaster, CePin>::len_;

template<
    typename SpiMaster,
    typename CePin
    >
uint32_t MotorsController<SpiMaster, CePin>::last_time_;

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
