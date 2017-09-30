#ifndef MOTORS_CONTROLLER_CLIENT_H
#define MOTORS_CONTROLLER_CLIENT_H

//TODO переименовать файл в motors_controller

#include "motors_controller_twi_face.h"

template<
    typename TwiMaster
    >
class MotorsController
{
public:
    static inline void init()
    {
        left_real_speed_    = 0;
        right_real_speed_   = 0;
        left_counter_       = 0;
        right_counter_      = 0;
    }

    static inline void update()
    {
        // real speed
        request_register(MCTF_REG_LEFT_SPEED, 2);
        read_register(&left_real_speed_, 2);
        request_register(MCTF_REG_RIGHT_SPEED, 2);
        read_register(&right_real_speed_, 2);

        // counter
        request_register(MCTF_REG_LEFT_COUNTER, 4);
        read_register(&left_counter_, 4);
        request_register(MCTF_REG_RIGHT_COUNTER, 4);
        read_register(&right_counter_, 4);
    }

    static void set_left_speed(int16_t left)
    {
        write_register(MCTF_REG_LEFT_SPEED, left);
    }

    static void set_right_speed(int16_t right)
    {
        write_register(MCTF_REG_RIGHT_SPEED, right);
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
    static void request_register(uint8_t address, uint16_t size)
    {
        while(!TwiMaster::is_ready());

        buffer_[0] = address;
        TwiMaster::request_from(MCTF_DEVICE_ADDRESS, buffer_, 1, buffer_, size);
    }

    static bool read_register(void* var, uint16_t len)
    {
        while(!TwiMaster::is_ready());

        if(TwiMaster::available() == len){
            memcpy(var, buffer_, len);
            return true;
        }else{
            return false;
        }
    }

    static void write_register(uint8_t address, int16_t data)
    {
        while(!TwiMaster::is_ready());

        buffer_[0] = address;
        memcpy(buffer_+1, &data, 2);
        TwiMaster::write_to(MCTF_DEVICE_ADDRESS, buffer_, 3);
    }

private:
    static uint8_t buffer_[8];

    static int16_t  left_real_speed_;
    static int16_t  right_real_speed_;
    static uint32_t left_counter_;
    static uint32_t right_counter_;
};

template<
    typename TwiMaster
    >
uint8_t MotorsController<TwiMaster>::buffer_[8];

template<
    typename TwiMaster
    >
int16_t MotorsController<TwiMaster>::left_real_speed_;

template<
    typename TwiMaster
    >
int16_t MotorsController<TwiMaster>::right_real_speed_;

template<
    typename TwiMaster
    >
uint32_t MotorsController<TwiMaster>::left_counter_;

template<
    typename TwiMaster
    >
uint32_t MotorsController<TwiMaster>::right_counter_;

#endif
