#ifndef STACK_H
#define STACK_H

template <
    typename T,
    size_t MAX_SIZE
    >
class Stack
{
public:
    Stack()
    {
        count_ = 0;
    }

    void push(T value)
    {
        if(count_ != MAX_SIZE){
            buffer_[count_] = value;
            count_++;
        }
    }

    T top() const
    {
        return buffer_[count_ - 1];
    }

    void pop()
    {
        if(count_ != 0){
            count_--;
        }
    }

    size_t size() const
    {
        return count_;
    }

    bool isEmpty() const
    {
        return count_ == 0;
    }

private:
    T buffer_[MAX_SIZE];
    uint16_t count_;
};

#endif
