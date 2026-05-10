#pragma once
#include "../Member.h"
#include <string.h>
static const uint8_t LEFT_SERVO_COUNT = 3;
static const uint8_t RIGHT_SERVO_COUNT = 3;
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
