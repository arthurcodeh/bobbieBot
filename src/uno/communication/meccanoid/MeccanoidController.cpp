#include "MeccanoidController.h"
#include <HardwareSerial.h>
#include <WString.h>
#include <string.h>

void MeccanoidController::add_member(Membre* m) {
    if (memberCount < MAX_MEMBERS) {
        membres[memberCount++] = m;
    }
}

void MeccanoidController::update() const {
    if (!Serial.available()) return;

    String cmd = Serial.readStringUntil('&');
    cmd.trim();

    // Format attendu: "nom_membre index angle"
    // Exemple: "left 0 90"
    int firstSpace = cmd.indexOf(' ');
    if (firstSpace == -1) return;

    int secondSpace = cmd.indexOf(' ', firstSpace + 1);
    if (secondSpace == -1) return;

    String name = cmd.substring(0, firstSpace);
    String indexStr = cmd.substring(firstSpace + 1, secondSpace);
    String angleStr = cmd.substring(secondSpace + 1);

    int index = indexStr.toInt();
    int angle = angleStr.toInt();

    // Trouver et commander le membre correspondant
    for (uint8_t i = 0; i < memberCount; i++) {
        if (strcmp(membres[i]->getName(), name.c_str()) == 0) {
            membres[i]->setDestination(index, angle);
            break;
        }
    }
}
