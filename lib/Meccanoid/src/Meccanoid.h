#ifndef MECCANOID_H
#define MECCANOID_H

#include <Arduino.h>
#include "MeccanoidProtocol.h"
#include "MeccanoidServo.h"
#include "MeccanoidLED.h"

class Meccanoid {
public:
    Meccanoid(uint8_t pinData);
    void begin(unsigned long baud = 2400);
    MeccanoidServo getServo(uint8_t id);
    MeccanoidLED getLED();
private:
    MeccanoidProtocol *_proto;
};

#endif
