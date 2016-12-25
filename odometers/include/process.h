#ifndef PROCESS_H
#define PROCESS_H

class Process
{
public:
    virtual void init() = 0;
    virtual void run() = 0;
    virtual void handleMessages() = 0;

private:

};

#endif
