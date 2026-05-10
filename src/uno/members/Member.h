#pragma once
#include "ServoSpec.h"
#include <Meccanoid.h>

#define MAX_SERVOS 10

class Membre {
protected:
    int pin;
    const char* name;
    Meccanoid meccanoid;
    ServoSpec servos[MAX_SERVOS];
    uint8_t servoCount;
    MeccanoidServo* servoInstances[MAX_SERVOS];
    int vitesse = 5;

public:
    Membre(int pin, const char* name, const ServoSpec* specs, uint8_t count);

    const char* getName() const;
    void setDestination(uint8_t index, int angle);
    void move();
};
