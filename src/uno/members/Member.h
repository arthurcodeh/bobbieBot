// File: Member.h
// Role : Classe de base commune à tous les membres du robot (Tete, Bras, ...)
//
// Un Membre contient :
//   - un ou plusieurs servos, chacun avec ses limites et son état
//   - une méthode setDestination() pour fixer un angle cible
//   - une méthode move() à appeler dans loop() pour interpoler vers la cible

#pragma once

#include "ServoSpec.h"
#include <Meccanoid.h>
#include "../config/RobotConfig.h"

// Nombre maximum de servos par membre
#define MAX_SERVOS 4

class Membre {
protected:
    int         pin;
    const char* name;
    Meccanoid   meccanoid;
    ServoSpec   servos[MAX_SERVOS];
    uint8_t     servoCount;

    // ✅ Initialisé à nullptr pour éviter un delete sur adresse aléatoire
    MeccanoidServo* servoInstances[MAX_SERVOS] = { nullptr };

    int vitesse = SERVO_DEFAULT_SPEED;

public:
    Membre(int pin, const char* name, const ServoSpec* specs, uint8_t count);

    // Retourne le nom logique du membre (ex: "head", "left")
    const char* getName() const;

    // Fixe l'angle cible du servo à l'index donné (clampé dans [min, max])
    void setDestination(uint8_t index, int angle);

    // À appeler dans loop() : fait avancer chaque servo d'un pas vers sa destination
    void move();
};