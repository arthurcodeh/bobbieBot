// File: Member.h
// Role : Classe de base commune à tous les membres du robot (Tete, Bras, ...)
//
// Un Membre contient :
//   - un ou plusieurs servos, chacun avec ses limites et son état
//   - une méthode setDestination() pour fixer un angle cible
//   - une méthode move() à appeler dans loop() pour interpoler vers la cible

#pragma once

#include "ServoSpec.h"
#include <Meccanoid.h>   // ← lib officielle
#include "../config/RobotConfig.h"

#define MAX_SERVOS 4

class Membre {
protected:
    int         pin;
    const char* name;
    Chain       chain;
    uint8_t     servoCount;
    int         vitesse = SERVO_DEFAULT_SPEED;

    // Specs mécaniques (min, max, position, destination)
    struct ServoState {
        int min;
        int max;
        int position    = 90;
        int destination = 90;
        bool wasMoving = false;
    };
    ServoState states[MAX_SERVOS];

public:
    Membre(int pin, const char* name, const ServoSpec* specs, uint8_t count);

    const char* getName() const;
    void setDestination(uint8_t index, int angle);
    void move();

private:
    void updateLED(uint8_t index, bool moving);
};