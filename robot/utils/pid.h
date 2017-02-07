#ifndef PID_H
#define PID_H

class Pid
{
public:
    Pid();
    Pid(float i_max, float i_min, float p, float i, float d);
    float update(float x, float y);


    float i_max;
    float i_min;
    float i_gain;
    float p_gain;
    float d_gain;

private:
    float i_state_;
    float max_value_;
    float min_value_;
};

#endif
