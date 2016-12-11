#ifndef PID_H
#define PID_H

class Pid
{
public:
    Pid();
    double update(double x, double y);

private:
    double i_state_;
    double i_max_;
    double i_min_;
    double p_gain_;
    double i_gain_;
    double d_gain_;
};

#endif
