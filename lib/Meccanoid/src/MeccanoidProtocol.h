#ifndef MECCANOID_PROTOCOL_H
#define MECCANOID_PROTOCOL_H

#include <Arduino.h>

class MeccanoidProtocol {
public:
    MeccanoidProtocol(Stream &serial, uint8_t pinData);
    void begin(unsigned long baud = 2400);
    void sendPacket(uint8_t *data, size_t len);
    uint8_t receiveByte();
private:
    Stream *_serial;
    uint8_t _pinData;
    void sendByte(uint8_t b);
    uint8_t receiveRawByte();
    static const unsigned int BIT_DELAY_US = 417;  // pour 2400 bauds
};

#endif
