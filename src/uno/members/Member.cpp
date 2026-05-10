// File: Member.cpp
// Role: Implémentation de la classe de base Membre
#include "Member.h"

Membre::Membre(int pin, const char* name, const ServoSpec* specs, uint8_t count)
    : pin(pin), name(name), meccanoid(pin), servoCount(count)
{
    // Sécurité : on ne dépasse jamais MAX_SERVOS
    if (servoCount > MAX_SERVOS) servoCount = MAX_SERVOS;

    meccanoid.begin(2400);

    for (uint8_t i = 0; i < servoCount; i++) {
        // Copie les specs (min, max, position, destination)
        servos[i] = specs[i];


        // Sécurité : on s'assure de ne pas avoir d'instance déjà créée (libère la mémoire si besoin)
        delete servoInstances[i]; // (libere la mémoire mais pointe tjrs vers l'instance, d'où la ligne suivante)
        servoInstances[i] = nullptr; // on s'assure de ne pas avoir de pointeur vers une instance supprimée

        // Récupère le servo physique depuis la lib Meccanoid et crée une instance de notre wrapper MeccanoidServo
        MeccanoidServo servo = meccanoid.getServo(i);
        servoInstances[i]    = new MeccanoidServo(servo);
        servos[i].servo      = servoInstances[i];

        // Position et destination de départ : centre
        servos[i].position    = 90;
        servos[i].destination = 90;
    }
}

const char* Membre::getName() const {
    return name;
}

void Membre::setDestination(uint8_t index, int angle) {
    if (index >= servoCount) return;

    ServoSpec& spec = servos[index];

    // Clamp : on force l'angle dans les limites autorisées
    if (angle < spec.min) angle = spec.min;
    if (angle > spec.max) angle = spec.max;

    spec.destination = angle;
}

void Membre::move() {
    for (uint8_t i = 0; i < servoCount; i++) {
        ServoSpec& spec = servos[i];

        // Rien à faire si déjà à destination
        if (spec.position == spec.destination) continue;

        int diff = spec.destination - spec.position;

        if (abs(diff) <= vitesse) {
            // On est assez proche : on se cale directement sur la cible
            spec.position = spec.destination;
        } else {
            // On avance d'un pas dans la bonne direction
            spec.position += (diff > 0) ? vitesse : -vitesse;
        }

        // Envoi de la nouvelle position au servo physique
        if (spec.servo != nullptr) {
            spec.servo->move(spec.position);
        }
    }
}
