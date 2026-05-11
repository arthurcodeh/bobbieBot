// File: Member.cpp
// Role : Implémentation de la classe de base Membre

#include "Member.h"

Membre::Membre(int pin, const char* name, const ServoSpec* specs, uint8_t count)
    : pin(pin), name(name), meccanoid(pin), servoCount(count)
{
    // Sécurité : on ne dépasse jamais MAX_SERVOS
    if (servoCount > MAX_SERVOS) servoCount = MAX_SERVOS;

    meccanoid.begin(2400);

    for (uint8_t i = 0; i < servoCount; i++) {
        // Copie les specs statiques (min, max, position, destination)
        servos[i] = specs[i];

        // ✅ servoInstances[i] est déjà nullptr grâce à l'initialisation dans le header.
        //    On n'appelle jamais delete sur un pointeur non initialisé.
        MeccanoidServo servo = meccanoid.getServo(i);
        servoInstances[i]    = new MeccanoidServo(servo);
        servos[i].servo      = servoInstances[i];

        // Position de départ : centre
        servos[i].position    = 90;
        servos[i].destination = 90;
    }
}

const char* Membre::getName() const {
    return name;
}

void Membre::setDestination(uint8_t index, int angle) {
    if (index >= servoCount) {
        Serial.print(F("[Membre] ERREUR : index servo invalide ("));
        Serial.print(index);
        Serial.print(F(") pour '"));
        Serial.print(name);
        Serial.println(F("'"));
        return;
    }

    ServoSpec& spec = servos[index];

    // Clamp : force l'angle dans les limites mécaniques autorisées
    int angleClampe = angle;
    if (angle < spec.min) angleClampe = spec.min;
    if (angle > spec.max) angleClampe = spec.max;

    if (angleClampe != angle) {
        Serial.print(F("["));
        Serial.print(name);
        Serial.print(F("] AVERTISSEMENT : angle "));
        Serial.print(angle);
        Serial.print(F("° contraint à "));
        Serial.print(angleClampe);
        Serial.println(F("°"));
    }

    spec.destination = angleClampe;

    Serial.print(F("["));
    Serial.print(name);
    Serial.print(F("] Servo "));
    Serial.print(index);
    Serial.print(F(" → destination fixée à "));
    Serial.print(spec.destination);
    Serial.println(F("°"));
}

void Membre::move() {
    for (uint8_t i = 0; i < servoCount; i++) {
        ServoSpec& spec = servos[i];

        // Rien à faire si déjà à destination
        if (spec.position == spec.destination) continue;

        int diff             = spec.destination - spec.position;
        int anciennePosition = spec.position;

        if (abs(diff) <= vitesse) {
            // Assez proche : on se cale directement sur la cible
            spec.position = spec.destination;
        } else {
            // On avance d'un pas dans la bonne direction
            spec.position += (diff > 0) ? vitesse : -vitesse;
        }

        Serial.print(F("["));
        Serial.print(name);
        Serial.print(F("] Servo "));
        Serial.print(i);
        Serial.print(F(" : "));
        Serial.print(anciennePosition);
        Serial.print(F("° → "));
        Serial.print(spec.position);
        Serial.print(F("° (cible : "));
        Serial.print(spec.destination);
        Serial.println(F("°)"));

        // Envoi de la nouvelle position au servo physique
        if (spec.servo != nullptr) {
            spec.servo->move(spec.position);
        }
    }
}