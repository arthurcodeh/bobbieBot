#include "MeccanoidProtocol.h"

MeccanoidProtocol::MeccanoidProtocol(Stream &serial, uint8_t pinData) {
    _serial = &serial;
    _pinData = pinData;
}

void MeccanoidProtocol::begin(unsigned long baud) {
    pinMode(_pinData, OUTPUT);
    digitalWrite(_pinData, HIGH); // repos = HIGH
    delay(100);
}

void MeccanoidProtocol::sendByte(uint8_t b) {
    pinMode(_pinData, OUTPUT);

    // Start bit
    digitalWrite(_pinData, LOW);
    delayMicroseconds(BIT_DELAY_US);

    // 8 bits de données, LSB en premier
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(_pinData, (b >> i) & 0x01 ? HIGH : LOW);
        delayMicroseconds(BIT_DELAY_US);
    }

    // Stop bit
    digitalWrite(_pinData, HIGH);
    delayMicroseconds(BIT_DELAY_US);
}

void MeccanoidProtocol::sendPacket(uint8_t *data, size_t len) {
    noInterrupts(); // ✅ critique pour le timing
    for (size_t i = 0; i < len; i++) {
        sendByte(data[i]);
    }
    interrupts();
}

uint8_t MeccanoidProtocol::receiveByte() {
    return receiveRawByte();
}

uint8_t MeccanoidProtocol::receiveRawByte() {
    pinMode(_pinData, INPUT);
    delayMicroseconds(BIT_DELAY_US); // attendre start bit

    uint8_t result = 0;
    for (uint8_t i = 0; i < 8; i++) {
        result |= (digitalRead(_pinData) << i);
        delayMicroseconds(BIT_DELAY_US);
    }

    pinMode(_pinData, OUTPUT);
    digitalWrite(_pinData, HIGH); // repasser en mode émission
    return result;
}