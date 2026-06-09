/**
 * @file Arm.h
 * @brief Classe représentant un bras du robot, avec ses spécifications de servos.
 *
 * Cette classe hérite de Membre et initialise les spécifications des servos en fonction du côté (gauche ou droit).
 * Les spécifications des servos sont définies dans des tableaux statiques pour chaque côté,
 * et sont utilisées pour configurer les servos lors de l'initialisation du membre.
 *
 * @see Arm.cpp
 * @see Membre
 */

#pragma once

#include "../Member.h"
#include "uno/config/Pins.h"
#include <string.h>

/**
 * Nombre de servos pour chaque bras, défini dans RobotConfig.h
 */
static const uint8_t LEFT_SERVO_COUNT = SERVOS_ARM_LEFT;
static const uint8_t RIGHT_SERVO_COUNT = SERVOS_ARM_RIGHT;

// --- CLASSE BRAS ENFANT DE MEMBRE ---
/**
 * @brief Classe représentant un bras du robot, avec ses spécifications de servos.
 *
 * Cette classe hérite de Membre et initialise les spécifications des servos en fonction du côté (gauche ou droit).
 * Les spécifications des servos sont définies dans des tableaux statiques pour chaque côté,
 * et sont utilisées pour configurer les servos lors de l'initialisation du membre.

 */
class Arm : public Member {
public:
    Arm(const int pin, const char* side)
        : Member(
            pin,
            side,
            (strcmp(side, "left") == 0) ? leftSpecs : rightSpecs,
            (strcmp(side, "left") == 0) ? LEFT_SERVO_COUNT : RIGHT_SERVO_COUNT
        ) {}

private:
    static ServoSpec leftSpecs[LEFT_SERVO_COUNT];
    static ServoSpec rightSpecs[RIGHT_SERVO_COUNT];
};
