// File: Member.h
// Role: Classe de base commune à tous les membres du robot (Tete, Bras, ...)
//
// Un Membre contient :
//   - un ou plusieurs servos, chacun avec ses limites et son état
//   - une méthode setDestination() pour fixer un angle cible
//   - une méthode move() à appeler dans loop() pour interpoler vers la cible
#pragma once
#include "ServoSpec.h"
#include <Meccanoid.h>

// Nombre max de servos par membre
#define MAX_SERVOS 4


class Membre {
protected:
    int pin;
    const char* name;
    Meccanoid meccanoid;
    ServoSpec servos[MAX_SERVOS];
    uint8_t servoCount;
    MeccanoidServo* servoInstances[MAX_SERVOS];
    int vitesse = 5; // vitesse d'interpolation (plus petit = plus lent)

public:
    Membre(int pin, const char* name, const ServoSpec* specs, uint8_t count);

    const char* getName() const; // retourne le nom du membre (ex: "head", "left_arm")
    void setDestination(uint8_t index, int angle); // fixe l'angle cible pour le servo d'index donné
    void move(); // à appeler dans loop() pour faire bouger les servos vers leurs destinations
};
