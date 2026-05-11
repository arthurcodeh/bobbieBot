// File: Member.cpp
// Role : Implémentation de la classe de base Membre

#include "Member.h"

Membre::Membre(int pin, const char* name, const ServoSpec* specs, uint8_t count)
    : pin(pin), name(name), chain(pin), servoCount(count)
{
    if (servoCount > MAX_SERVOS) servoCount = MAX_SERVOS;

    for (uint8_t i = 0; i < servoCount; i++) {
        states[i].min         = specs[i].min;
        states[i].max         = specs[i].max;
        states[i].position    = 90;
        states[i].destination = 90;
    }
}

const char* Membre::getName() const {
    return name;
}

void Membre::setDestination(uint8_t index, int angle) {
    if (index >= servoCount) return;

    int clamped = angle;
    if (angle < states[index].min) clamped = states[index].min;
    if (angle > states[index].max) clamped = states[index].max;

    if (clamped != angle) {
        Serial.print(F("["));
        Serial.print(name);
        Serial.print(F("] AVERTISSEMENT : angle "));
        Serial.print(angle);
        Serial.print(F("° contraint à "));
        Serial.print(clamped);
        Serial.println(F("°"));
    }

    states[index].destination = clamped;

    Serial.print(F("["));
    Serial.print(name);
    Serial.print(F("] Servo "));
    Serial.print(index);
    Serial.print(F(" → destination fixée à "));
    Serial.print(clamped);
    Serial.println(F("°"));
}

void Membre::move() {
    for (uint8_t i = 0; i < servoCount; i++) {
        ServoState& s = states[i];

        if (s.position == s.destination) continue;

        int diff = s.destination - s.position;
        int old  = s.position;

        if (abs(diff) <= vitesse) {
            s.position = s.destination;
        } else {
            s.position += (diff > 0) ? vitesse : -vitesse;
        }

        chain.getServo(i).setPosition(s.position);

        Serial.print(F("["));
        Serial.print(name);
        Serial.print(F("] Servo "));
        Serial.print(i);
        Serial.print(F(" : "));
        Serial.print(old);
        Serial.print(F("° → "));
        Serial.print(s.position);
        Serial.print(F("° (cible : "));
        Serial.print(s.destination);
        Serial.println(F("°)"));
    }

    // toujours appelé — obligatoire pour le protocole SM
    chain.update();
}