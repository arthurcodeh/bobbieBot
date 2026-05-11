#include "MeccanoidServo.h"

MeccanoidServo::MeccanoidServo(MeccanoidProtocol &proto, uint8_t servoId) {
    _proto = &proto;
    _id = servoId;
}

void MeccanoidServo::move(uint8_t position) {
    // mapping de 0–180° vers 0–255 (ou 0–0xD0 pour éviter les valeurs extrêmes)
    uint8_t mapped = map(position, 0, 180, 0, 0xD0);

    uint8_t packet[4];
    packet[0] = 0xFF;     // broadcast ID (ou _id pour adresse spécifique)
    packet[1] = (_id << 2) | 0x01;     // cmd: move
    packet[2] = mapped;
    packet[3] = buildChecksum(packet, 3);

    Serial.print("[Servo] Paquet envoyé : ");
    for (int i = 0; i < 4; i++) {
        Serial.print("0x");
        Serial.print(packet[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    _proto->sendPacket(packet, 4);
    delay(10);
}

void MeccanoidServo::setSpeed(uint8_t speed) {
    uint8_t packet[4];
    packet[0] = 0xFF;
    packet[1] = (_id << 2) | 0x02;;     // cmd: speed
    packet[2] = speed;
    packet[3] = buildChecksum(packet, 3);
    _proto->sendPacket(packet, 4);
    delay(10);
}

uint8_t MeccanoidServo::getPosition() {
    uint8_t packet[3];
    packet[0] = 0xFF;
    packet[1] = (_id << 2) | 0x00;     // cmd: get position
    packet[2] = buildChecksum(packet, 2);

    _proto->sendPacket(packet, 3);
    return _proto->receiveByte();
}

uint8_t MeccanoidServo::buildChecksum(uint8_t *data, size_t len) {
    uint16_t checksum = 0;
    for (size_t i = 0; i < len; i++) {
        checksum += data[i];
    }
    // simple checksum : sum & 0xFF
    return checksum;
}
