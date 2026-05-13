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
        states[i].wasMoving   = false;
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

void Membre::updateLED(uint8_t index, bool moving) {
    // Chaque servo a sa LED associée au même index dans la chain.
    // Rouge (7,0,0) = en mouvement — Rouge vif, facilement visible.
    // Vert  (0,7,0) = au repos     — Vert vif, état nominal.
    // fadetime = 0 : changement instantané (pas de transition floue).
    MeccanoServo servo = chain.getServo(index);
    if (moving) {
        servo.setColor(1, 0, 0); // Rouge
    } else {
        servo.setColor(0, 1, 0); // Vert
    }
}

void Membre::move() {
    for (uint8_t i = 0; i < servoCount; i++) {
        ServoState& s = states[i];

        // --- Cas 1 : le servo est déjà à sa cible ---
        if (s.position == s.destination) {

            // Si on vient juste d'arriver (transition mouvement → repos),
            // on met le servo en mode limiteur (économie d'énergie, pas de vibration)
            // et on repasse la LED au vert.
            if (s.wasMoving) {
                chain.getServo(i).setLim(true);
                updateLED(i, false);
                s.wasMoving = false;

                Serial.print(F("["));
                Serial.print(name);
                Serial.print(F("] Servo "));
                Serial.print(i);
                Serial.println(F(" → cible atteinte, passage au repos ✓"));
            }

            continue; // rien d'autre à faire pour ce servo
        }

        // --- Cas 2 : le servo doit encore bouger ---

        // Au premier tick de mouvement, on sort le servo du mode repos
        // et on allume la LED en rouge.
        if (!s.wasMoving) {
            chain.getServo(i).setLim(false);
            updateLED(i, true);
            s.wasMoving = true;
        }

        // Interpolation : on avance d'un pas de `vitesse` vers la cible
        int old  = s.position;
        int diff = s.destination - s.position;

        if (abs(diff) <= vitesse) {
            s.position = s.destination; // dernier pas : on se cale exactement
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
}