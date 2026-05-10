#ifndef MECCANOID_LED_H
#define MECCANOID_LED_H

#include <Arduino.h>
#include "MeccanoidProtocol.h"

class MeccanoidLED {
public:
    MeccanoidLED(MeccanoidProtocol &proto);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
private:
    MeccanoidProtocol *_proto;
};

#endif
