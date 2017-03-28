#include "robot.h"

void Robot::init()
{
    //hardware
    Serial::init();
    Time::init();
    TwiMaster::init();
    MowerMotor::init();
    Adc::init();

    sei();

    //processes
    terminal_.init();
    button_.init();
    bumper_.init();
    perimeter_sensor_.init();
    led_indicator_.init();
    // algorithm_.init();
    mower_.init();
    drive_motors_.init();

    //state
    status_ = 0;

    //mower
    mower_msg_.speed = 255; //max
    mower_msg_.is_enable = false;

    //drive motors
    drive_motors_msg_.is_new_data = true;
    drive_motors_msg_.left_set_speed = 0;
    drive_motors_msg_.right_set_speed = 0;
    drive_motors_msg_.left_real_speed = 0;
    drive_motors_msg_.right_real_speed = 0;
    drive_motors_msg_.left_counter = 0;
    drive_motors_msg_.right_counter = 0;

    //bumper
    bumper_msg_.left = 0;
    bumper_msg_.left_min = 557;
    bumper_msg_.left_max = 729;

    bumper_msg_.right = 0;
    bumper_msg_.right_min = 403;
    bumper_msg_.right_max = 577;

    bumper_msg_.is_calibration = false;
    bumper_msg_.is_save = false;

    //perimeter sensor
    perimeter_sensor_msg_.signal_offset = 512;

    perimeter_sensor_msg_.lps = 0;

    perimeter_sensor_msg_.magnitude = 0;
    perimeter_sensor_msg_.smooth_magnitude = 0;
    perimeter_sensor_msg_.is_inside = false;
    perimeter_sensor_msg_.is_new_data = false;


    perimeter_sensor_msg_.is_invert_sigcode = false;
    perimeter_sensor_msg_.is_offset_calibration_mode = false;

    //algorithm
    pid_ = Pid(300.0, -300.0, 1.0, 0.0, 0.0);

    //begin
    state_.push(state_wait);
}

void Robot::loop()
{
    // sub fsm
    terminal_.run(&status_, &mower_msg_, &drive_motors_msg_, &bumper_msg_,
            &perimeter_sensor_msg_);

    button_.run(&status_);

    bumper_.run(&bumper_msg_);

    perimeter_sensor_.run(&perimeter_sensor_msg_);

    led_indicator_.run(&status_);

    mower_.run(&mower_msg_);

    drive_motors_.run(&drive_motors_msg_);

    //tick
    uint8_t current_state = state_.top();
    state_.pop();

    switch(current_state)
    {
    case state_wait:
        state_.push(state_perimeter);
        break;

    case state_stop:
        stopMotors();
        break;

    case state_perimeter:
        if(isStuffOnWay()){
            stopMotors();
            state_.push(state_perimeter);
            state_.push(state_avoid_stuff);
        }
        perimeterFlow();
        state_.push(state_perimeter);
        break;

    case state_avoid_stuff:
        if(isStuffOnWay()){
            int16_t left = 0;
            if(bumper_msg_.left > 50){
                left = -WORK_SPEED;
            }
            int16_t right = 0;
            if(bumper_msg_.right > 50){
                right = -WORK_SPEED;
            }
            drive_motors_msg_.setSpeed(left, right);

            state_.push(state_avoid_stuff);
        }
        break;

    case state_fill:
        break;

    default:
        break;
    }
}

void Robot::stopMotors()
{
    drive_motors_msg_.setSpeed(0, 0);
}

void Robot::perimeterFlow()
{
    int16_t left_speed = 0;
    int16_t right_speed = 0;

    int16_t value = 0;
    static int16_t last_value = 0;
    static uint32_t last_time = 0;

    if(perimeter_sensor_msg_.magnitude < 0){
        value = -1; //outside
        PORTB &= ~(1<<5);
    }else if(perimeter_sensor_msg_.magnitude > 0){
        value = 1; //inside
        PORTB |= (1<<5);
    }else{
        value = 0; //on wire
    }

    if(value != last_value){
        last_time = Time::now();
        last_value = value;
    }

    int32_t perimeter_value = (Time::now() - last_time) / 10 * value;

    int16_t correction  = (int16_t)pid_.update(0.0, (float)perimeter_value);
    if(WORK_SPEED + correction > MAX_SPEED){
        left_speed = MAX_SPEED;
    }else if(WORK_SPEED + correction < -MAX_SPEED){
        left_speed = -MAX_SPEED;
    }else{
        left_speed = WORK_SPEED + correction;
    }

    if(WORK_SPEED - correction > MAX_SPEED){
        right_speed = MAX_SPEED;
    }else if(WORK_SPEED - correction < -MAX_SPEED){
        right_speed = -MAX_SPEED;
    }else{
        right_speed = WORK_SPEED - correction;
    }

    drive_motors_msg_.setSpeed(left_speed, right_speed);

}

bool Robot::isStuffOnWay()
{
    return bumper_msg_.left >= 100 || bumper_msg_.right >= 100;
}

void Robot::avoidStuff()
{

}
