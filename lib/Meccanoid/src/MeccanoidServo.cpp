#include "MeccanoidServo.h"

MeccanoidServo::MeccanoidServo(MeccanoidProtocol &proto, uint8_t servoId) {
    _proto = &proto;
    _id = servoId;
}

void MeccanoidServo::move(uint8_t position) {
    uint8_t packet[4];
    packet[0] = _id;
    packet[1] = 0x01;     // cmd: move
    packet[2] = position;
    packet[3] = buildChecksum(packet, 3);
    _proto->sendPacket(packet, 4);
}

void MeccanoidServo::setSpeed(uint8_t speed) {
    uint8_t packet[4];
    packet[0] = _id;
    packet[1] = 0x04;     // cmd: speed
    packet[2] = speed;
    packet[3] = buildChecksum(packet, 3);
    _proto->sendPacket(packet, 4);
}

uint8_t MeccanoidServo::getPosition() {
    uint8_t packet[3];
    packet[0] = _id;
    packet[1] = 0x02;     // cmd: get position
    packet[2] = buildChecksum(packet, 2);
    _proto->sendPacket(packet, 3);
    return _proto->receiveByte();
}

uint8_t MeccanoidServo::buildChecksum(uint8_t *data, size_t len) {
    uint16_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += data[i];
    }
    // simple checksum : sum & 0xFF
    return (uint8_t)(sum & 0xFF);
}
