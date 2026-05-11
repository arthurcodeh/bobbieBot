#ifndef MECCANOID_SERVO_H
#define MECCANOID_SERVO_H

#include <Arduino.h>
#include "MeccanoidProtocol.h"

class MeccanoidServo {
public:
    MeccanoidServo(MeccanoidProtocol &proto, uint8_t servoId);
    void move(uint8_t position);     // position 0–255 (ou 0–180°, selon mapping)
    void setSpeed(uint8_t speed);
    uint8_t getPosition();
private:
    MeccanoidProtocol *_proto;
    uint8_t _id;
    uint8_t buildChecksum(uint8_t *data, size_t len);
};

#endif
