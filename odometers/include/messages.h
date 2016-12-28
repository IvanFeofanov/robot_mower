#ifndef MESSAGES_H
#define MESSAGES_H

typedef struct Msg
{
    char    state;
    void*   param_ptr;

    enum MsgStates
    {
        INACTIVE,
        INSTALLED,
        ACTIVE
    };

}Msg;

template<   int MAX_MESSAGES,
            int MAX_BROADCAST_MESSAGES
        >
class MessagesT
{
public:
    static void init()
    {
        for(uint16_t i = 0; i < MAX_MESSAGES; i++)
            messages_[i].state = Msg::INACTIVE;

        for(uint16_t i = 0; i < MAX_BROADCAST_MESSAGES; i++)
            broadcast_messages_[i].state = Msg::INACTIVE;
    }

    static void run()
    {
        for(uint16_t i = 0; i < MAX_MESSAGES; i++){
            if(messages_[i].state == Msg::ACTIVE)
                messages_[i].state = Msg::INACTIVE;
            if(messages_[i].state == Msg::INSTALLED)
                messages_[i].state = Msg::ACTIVE;
        }

        for(uint16_t i = 0; i < MAX_BROADCAST_MESSAGES; i++)
        {
            if(broadcast_messages_[i].state == Msg::ACTIVE)
                broadcast_messages_[i].state = Msg::INACTIVE;
            if(broadcast_messages_[i].state == Msg::INSTALLED)
                broadcast_messages_[i].state = Msg::ACTIVE;
        }
    }

    static void send(uint16_t msg_id, void* param_ptr)
    {
        messages_[msg_id].state     = Msg::INSTALLED;
        messages_[msg_id].param_ptr = param_ptr;
    }

    static void sendBroadcast(uint16_t msg_id, void* param_ptr)
    {
        broadcast_messages_[msg_id].state     = Msg::INSTALLED;
        broadcast_messages_[msg_id].param_ptr = param_ptr;
    }

    static bool get(uint16_t msg_id, void** param_ptr)
    {
        if(messages_[msg_id].state == Msg::ACTIVE){
            *param_ptr = messages_[msg_id].param_ptr;
            messages_[msg_id].state = Msg::INACTIVE;
            return true;
        }

        return false;
    }

    static bool getBroadcast(uint16_t msg_id, void** param_ptr)
    {
        if(broadcast_messages_[msg_id].state == Msg::INSTALLED){
            *param_ptr = broadcast_messages_[msg_id].param_ptr;
            return true;
        }

        return false;
    }

private:
    static Msg messages_[MAX_MESSAGES];
    static Msg broadcast_messages_[MAX_BROADCAST_MESSAGES];

};

template<
        int MAX_MESSAGES,
        int MAX_BROADCAST_MESSAGES
        >
Msg MessagesT<MAX_MESSAGES, MAX_BROADCAST_MESSAGES>::messages_[MAX_MESSAGES];

template<
        int MAX_MESSAGES,
        int MAX_BROADCAST_MESSAGES
        >
Msg MessagesT<MAX_MESSAGES, MAX_BROADCAST_MESSAGES>::
    broadcast_messages_[MAX_BROADCAST_MESSAGES];

typedef MessagesT<10, 5> Messages;

#endif
