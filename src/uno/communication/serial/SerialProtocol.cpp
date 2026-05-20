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

// Instanciation du SoftwareSerial pour l'ESP32 (Arduino Uno) sur les pins définis dans Pins.h
SoftwareSerial SerialProtocol::espSerial(RX_ESP32,TX_ESP32);

void SerialProtocol::begin() {
    espSerial.begin(SERIAL_BAUD_ESP32);
    Serial.println(F("[SerialProtocol] Initialisé — écoute Serial (debug) + Serial1 (ESP32) ✓"));
}

bool SerialProtocol::read(Command& out) {
    String line = "";

    // Priorité à l'ESP32 (Serial1) pour éviter de mélanger les commandes debug et ESP32
    if (espSerial.available()) {
        line = espSerial.readStringUntil('\n');
    } else if (Serial.available()) {
        line = Serial.readStringUntil('\n');
    } else {
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
        return false;
    }

    // Log différencié selon le type de commande reçue
    Serial.print(F("[SerialProtocol] Parsé → membre='"));
    Serial.print(out.member);
    if (out.action[0] != '\0') {
        Serial.print(F("' action='"));
        Serial.print(out.action);
    } else {
        Serial.print(F("' servo="));
        Serial.print(out.servoIndex);
        Serial.print(F(" angle="));
        Serial.print(out.angle);
    }
    Serial.println(F("'"));

    return true;
}

Command SerialProtocol::parse(const String& line) {
    Command cmd;
    cmd.valid      = false;
    cmd.servoIndex = -1;
    cmd.angle      = -1;
    cmd.action[0]  = '\0';
    cmd.member[0]  = '\0';

    //sépare le nom du membre du reste
    int firstSpace = line.indexOf(' ');
    if (firstSpace == -1) return cmd; // SI aucun espace => commande incomplète

    // Extraire le nom du membre ("head", "led", ...)
    String memberStr = line.substring(0, firstSpace);
    memberStr.toCharArray(cmd.member, sizeof(cmd.member));

    String rest = line.substring(firstSpace + 1);
    rest.trim();
    if (rest.length() == 0) return cmd; // Rien après le nom du membre

    // --- Commande action : "eyes red" / "eyes off" ---
    // Un seul token restant = pas de servo index ni d'angle
    if (rest.indexOf(' ') == -1) {
        rest.toCharArray(cmd.action, sizeof(cmd.action));
        cmd.valid = true;
        return cmd;
    }


    // --- Commande servo : "head 0 120" ---
    int secondSpace = rest.indexOf(' ');
    String indexStr = rest.substring(0, secondSpace);
    String angleStr = rest.substring(secondSpace + 1);
    angleStr.trim();

    if (angleStr.length() == 0) return cmd; // Angle manquant

    cmd.servoIndex = indexStr.toInt();
    cmd.angle      = angleStr.toInt();
    cmd.valid      = true;

    return cmd;
}