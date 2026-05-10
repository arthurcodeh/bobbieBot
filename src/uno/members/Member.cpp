#include "Member.h"

Membre::Membre(int pin, const char* name, const ServoSpec* specs, uint8_t count)
    : pin(pin), name(name), meccanoid(pin), servoCount(count)
{
    if (count > MAX_SERVOS) servoCount = MAX_SERVOS;

    meccanoid.begin(2400);

    for (uint8_t i = 0; i < servoCount; i++) {
        servos[i] = specs[i];
        MeccanoidServo servo = meccanoid.getServo(i);
        servoInstances[i] = new MeccanoidServo(servo);
        servos[i].servo = servoInstances[i];
        servos[i].position = 90;
        servos[i].destination = 90;
    }
}

const char* Membre::getName() const {
    return name;
}

void Membre::setDestination(uint8_t index, int angle) {
    if (index >= servoCount) return;

    ServoSpec& spec = servos[index];

    // Appliquer les limites
    if (angle < spec.min) angle = spec.min;
    if (angle > spec.max) angle = spec.max;

    spec.destination = angle;
}

void Membre::move() {
    for (uint8_t i = 0; i < servoCount; i++) {
        ServoSpec& spec = servos[i];
        if (spec.position != spec.destination) {
            // Calculer le prochain pas
            int diff = spec.destination - spec.position;

            if (abs(diff) <= vitesse) {
                // Arrivé à destination
                spec.position = spec.destination;
            } else {
                // Se déplacer progressivement
                spec.position += (diff > 0) ? vitesse : -vitesse;
            }

            // Envoyer la commande au servo physique
            if (spec.servo != nullptr) {
                spec.servo->move(spec.position);
            }
        }
    }
}
