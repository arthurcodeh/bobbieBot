#include "MeccanoidLED.h"

MeccanoidLED::MeccanoidLED(MeccanoidProtocol &proto) {
    _proto = &proto;
}

void MeccanoidLED::setColor(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t packet[5];
    packet[0] = 0xFF;       // header / start byte
    packet[1] = r;
    packet[2] = g;
    packet[3] = b;
    packet[4] = 0x00;       // checksum or padding (selon protocole exact)
    _proto->sendPacket(packet, 5);
}
