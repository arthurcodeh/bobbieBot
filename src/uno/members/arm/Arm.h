#pragma once
#include "../Member.h"
#include "uno/config/Pins.h"
#include <string.h>

static const uint8_t LEFT_SERVO_COUNT = SERVOS_ARM_LEFT;
static const uint8_t RIGHT_SERVO_COUNT = SERVOS_ARM_RIGHT;
class Bras : public Membre {
private:
    static ServoSpec leftSpecs[LEFT_SERVO_COUNT];
    static ServoSpec rightSpecs[RIGHT_SERVO_COUNT];

public:
    Bras(const int pin, const char* side)
        : Membre(
            pin,
            side,
            (strcmp(side, "left") == 0) ? leftSpecs : rightSpecs,
            RIGHT_SERVO_COUNT
        ) {}
};
