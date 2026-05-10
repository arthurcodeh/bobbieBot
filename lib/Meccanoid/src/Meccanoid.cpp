#include "Meccanoid.h"
#include <SoftwareSerial.h>

Meccanoid::Meccanoid(uint8_t pinData) {
    // On utilise SoftwareSerial ou la Serial hardware selon ton choix
    // Ici on suppose SoftwareSerial mais tu peux adapter
    static SoftwareSerial softSerial(pinData, pinData); // RX, TX sur la même pin
    softSerial.begin(2400);
    _proto = new MeccanoidProtocol(softSerial, pinData);
}

void Meccanoid::begin(unsigned long baud) {
    _proto->begin(baud);
}

MeccanoidServo Meccanoid::getServo(uint8_t id) {
    return MeccanoidServo(*_proto, id);
}

MeccanoidLED Meccanoid::getLED() {
    return MeccanoidLED(*_proto);
}
