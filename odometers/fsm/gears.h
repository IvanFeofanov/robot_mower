#ifndef GEARS_H
#define GEARS_H

#include "messages.h"

template<
    typename Drive,
    uint16_t VECTOR_MSG
    >
class Gears : public Process
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
