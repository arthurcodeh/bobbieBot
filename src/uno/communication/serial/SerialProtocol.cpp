// File : SerialProtocol.cpp
// Role: Communication entre l'arduino et l'esp 32

// Description : SerialProtocol sert à gérer la communication entre l'arduino et l'esp32,
// qui transmet les données à l'arduino pour le contrôle du robot


#include "SerialProtocol.h"

void SerialProtocol::begin(unsigned long baudRate) {
    Serial.begin(baudRate);
}

bool SerialProtocol::read(Command& out) {
    if (!Serial.available()) return false;

    // Lire jusqu'au saut de ligne (envoyé par l'ESP32 avec println)
    String line = Serial.readStringUntil('\n');
    line.trim(); // supprime \r et espaces superflus

    if (line.length() == 0) return false;

    out = parse(line);
    return out.valid;
}

Command SerialProtocol::parse(const String& line) {
    Command cmd;
    cmd.valid      = false;
    cmd.servoIndex = -1;
    cmd.angle      = -1;
    cmd.action[0]  = '\0';

    // Trouver le premier espace → sépare le nom du membre du reste
    int firstSpace = line.indexOf(' ');
    if (firstSpace == -1) return cmd; // commande incomplète

    // Extraire le nom du membre ("head", "led", ...)
    String memberStr = line.substring(0, firstSpace);
    memberStr.toCharArray(cmd.member, sizeof(cmd.member));

    String rest = line.substring(firstSpace + 1);
    rest.trim();

    // --- Commande LED : "led red" / "led off" ---
    if (memberStr == "led") {
        if (rest.length() == 0) return cmd;
        rest.toCharArray(cmd.action, sizeof(cmd.action));
        cmd.valid = true;
        return cmd;
    }

    // --- Commande servo : "head 0 120" ---
    int secondSpace = rest.indexOf(' ');
    if (secondSpace == -1) return cmd; // il manque l'angle

    String indexStr = rest.substring(0, secondSpace);
    String angleStr = rest.substring(secondSpace + 1);

    cmd.servoIndex = indexStr.toInt();
    cmd.angle      = angleStr.toInt();
    cmd.valid      = true;

    return cmd;
} // SerialProtocol