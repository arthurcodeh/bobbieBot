/**
 * @file SerialProtocol.cpp
 * @brief Implémentation du parser de commandes série.
 *
 * Protocole de commande attendu (envoyé par l'ESP32) :
 *   - Servo : "head 0 120"  → membre "head", servo 0, angle 120°
 *   - LED   : "led red"     → action "red" pour le membre "led"
 *
 * Ports série utilisés :
 *   - Serial  → debug USB (moniteur série du PC)
 *   - Serial1 → communication avec l'ESP32 (RX1/TX1 de l'Arduino)
 *
 * @note Sur Arduino Uno, Serial1 n'est pas disponible nativement.
 *       Utiliser un SoftwareSerial sur les pins définis dans Pins.h,
 *       ou un Arduino Mega qui expose Serial1 en hardware.
 */

#include "SerialProtocol.h"

void SerialProtocol::begin(unsigned long baudRate) {
    Serial1.begin(baudRate);
    Serial.println(F("[SerialProtocol] Initialisé — écoute Serial (debug) + Serial1 (ESP32) ✓"));
}

bool SerialProtocol::read(Command& out) {
    String line = "";

    // --- ESP32 via Serial1 ---
    if (Serial1.available()) {
        line = Serial1.readStringUntil('\n');
    }
    // --- debug manuel via moniteur série ---
    else if (Serial.available()) {
        line = Serial.readStringUntil('\n');
    }
    else {
        return false; // rien à lire sur aucun des deux ports
    }

    line.trim(); // supprime \r et espaces superflus
    if (line.length() == 0) return false;

    Serial.print("[SerialProtocol] Ligne reçue : '");
    Serial.print(line);
    Serial.println("'");

    out = parse(line);

    if (!out.valid) {
        Serial.println("[SerialProtocol] ERREUR : commande invalide ou mal formée");
    } else {
        Serial.print("[SerialProtocol] Commande parsée → membre='");
        Serial.print(out.member);
        Serial.print("' servoIndex=");
        Serial.print(out.servoIndex);
        Serial.print(" angle=");
        Serial.println(out.angle);
    }
    return out.valid;
}

Command SerialProtocol::parse(const String& line) {
    Command cmd;
    cmd.valid      = false;
    cmd.servoIndex = -1;
    cmd.angle      = -1;
    cmd.action[0]  = '\0';

    //sépare le nom du membre du reste
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
}