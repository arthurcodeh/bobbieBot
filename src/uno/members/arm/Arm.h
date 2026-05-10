#pragma once
#include "../Member.h"
#include <string.h>

class Bras : public Membre {
private:
    static ServoSpec leftSpecs[3];
    static ServoSpec rightSpecs[3];

public:
    Bras(const int pin, const char* side)
        : Membre(
            pin,
            side,
            (strcmp(side, "left") == 0) ? leftSpecs : rightSpecs,
            3
        ) {}
};
