#ifndef RING_BUFFER_H
#define RING_BUFFER_H

template<
    int      SIZE,
    typename T
    >
class RingBuffer
{
public:
    RingBuffer()
    {
        head_ = 0;
        tail_ = 0;
    }

    bool put(T value)
    {
        buffer_[head_] = value;
        if(head_ == SIZE-1)
            head_ = 0;
        else
            head_++;

        if(head_ == tail_){
            if(tail_ == SIZE - 1)
                tail_ = 0;
            else
                tail_++;
        }

        return true;
    }

    bool take(T* value_ptr)
    {
        //empty
        if(tail_ == head_){
            return false;
        }else{
            *value_ptr = buffer_[tail_];
        }

        if(tail_ == SIZE - 1)
            tail_ = 0;
        else
            tail_++;

        return true;
    }

    bool isEmpty()
    {
        return tail_ == head_;
    }

    uint16_t count()
    {
        if(head_ >= tail_)
            return head_ - tail_;
        else
            return SIZE - tail_ + head_ + 1;
    }

    void flush()
    {
        head_ = 0;
        tail_ = 0;
    }

private:
    T buffer_[SIZE];
    uint16_t head_;
    uint16_t tail_;
};



#endif
