#include "Arm.h"

ServoSpec Bras::leftSpecs[LEFT_SERVO_COUNT] = {
    {nullptr, 10, 170, 90, 90},
    {nullptr, 10, 180, 90, 90},
    {nullptr, 0, 90, 90, 90}
};

ServoSpec Bras::rightSpecs[RIGHT_SERVO_COUNT] = {
    {nullptr, 10, 170, 90, 90},
    {nullptr, 10, 180, 90, 90},
    {nullptr, 90, 180, 90, 90}
};