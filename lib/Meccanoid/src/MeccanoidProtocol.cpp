#include "MeccanoidProtocol.h"
#include <SoftwareSerial.h>

MeccanoidProtocol::MeccanoidProtocol(Stream &serial, uint8_t pinData) {
    _serial = &serial;
    _pinData = pinData;
}

void MeccanoidProtocol::begin(unsigned long baud) {
    // Si tu utilises SoftwareSerial, initialize le ici
    // Par défaut, on suppose que serial est déjà begin()
    pinMode(_pinData, OUTPUT);
    digitalWrite(_pinData, HIGH);
}

void MeccanoidProtocol::sendByte(uint8_t bytData) {
    pinMode(_pinData, OUTPUT);
    digitalWrite(_pinData, LOW);
    delayMicroseconds(BIT_DELAY_US);

    for (uint8_t mask = 1; mask; mask <<= 1) {
        if (bytData & mask) {
            digitalWrite(_pinData, HIGH);
        } else {
            digitalWrite(_pinData, LOW);
        }
        delayMicroseconds(BIT_DELAY_US);
    }

    digitalWrite(_pinData, HIGH);
    delayMicroseconds(BIT_DELAY_US);
    digitalWrite(_pinData, HIGH);
    delayMicroseconds(BIT_DELAY_US);
}

uint8_t MeccanoidProtocol::receiveRawByte() {
    pinMode(_pinData, INPUT);
    delay(2); // wait

    uint8_t result = 0;
    for (uint8_t mask = 1; mask; mask <<= 1) {
        unsigned long pulse = pulseIn(_pinData, HIGH, 2500);
        if (pulse > 400) {
            result |= mask;
        }
    }
    return result;
}

void MeccanoidProtocol::sendPacket(uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        sendByte(data[i]);
    }
}

uint8_t MeccanoidProtocol::receiveByte() {
    return receiveRawByte();
}
