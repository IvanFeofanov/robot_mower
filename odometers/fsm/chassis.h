#ifndef CHASSIS_H
#define CHASSIS_H

#include "messages.h"

template<
    typename Drive,
    uint16_t VECTOR_MSG
    >
class Chassis: public Process
{
public:
    void init()
    {
    }
    void run()
    {
        if(MsgHandler::get(VECTOR_MSG, 0))
        {
            Drive::setDirection(1);
        }
    }

    void handleMessages()
    {

    }
};

#endif
