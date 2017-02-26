#ifndef TWI_INTERFACE_H
#define TWI_INTERFACE_H

#include "mediator.h"


template<
    typename TWI
    >
class TwiInterface
{
public:
    static inline void init()
    {
        TWI::onReceive(&setValueCb);
        TWI::onRequest(&getValueCb);
    }

private:
    static void setValueCb(uint8_t* data, uint8_t length)
    {
        PORTC ^= (1<<2);

        switch(data[0])
        {
        case RPS:
            //left
            Mediator::set_rps[Mediator::LEFT_MOTOR] = (data[1] << 8) | data[2];
            //right
            Mediator::set_rps[Mediator::RIGHT_MOTOR] = (data[3] << 8) | data[4];
            // Mediator::set_rps[Mediator::RIGHT_MOTOR] = 90;
            break;

        default:
            break;
        }
    }

    static void getValueCb(uint8_t* request, uint8_t length)
    {
        PORTC ^= (1<<2);

        switch(request[0])
        {
        case RPS:
            //left
            buffer_[0] =  (Mediator::real_rps[Mediator::LEFT_MOTOR] >> 8);
            buffer_[1] =  Mediator::real_rps[Mediator::LEFT_MOTOR];

            //right
            buffer_[2] =  (Mediator::real_rps[Mediator::RIGHT_MOTOR] >> 8);
            buffer_[3] =  Mediator::real_rps[Mediator::RIGHT_MOTOR];

            TWI::write(buffer_, 4);
            break;

        case COUNTER:
            //left
            for(int i = 0; i < 4; i++){
                buffer_[i] = (Mediator::counter[Mediator::LEFT_MOTOR] >> 8*(3-i));
            }

            //right
            for(int i = 0; i < 4; i++){
                buffer_[4+i] = (Mediator::counter[Mediator::RIGHT_MOTOR] >> 8*(3-i));
            }

            TWI::write(buffer_, 8);
            break;

        default:
            break;
        }
    }

private:
    enum {
        RPS,
        COUNTER
    };

    static uint8_t buffer_[8];
};

template<typename TWI>
uint8_t TwiInterface<TWI>::buffer_[8];

#endif
