#ifndef PID_H
#define PID_H

class Pid
{
public:
    Pid();
    float update(float x, float y);

private:
    float i_state_;
    float i_max_;
    float i_min_;
    float i_gain_;

    float p_gain_;
    float d_gain_;

    float max_value_;
    float min_value_;
};

#endif
