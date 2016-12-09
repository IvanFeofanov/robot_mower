#include "drive_motors.h"

extern volatile uint32_t DriveMotors::counter_;

ISR(INT0_vect)
{
    DriveMotors::odometerInterrupt();
}
