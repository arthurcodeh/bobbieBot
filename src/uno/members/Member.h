/**
 * @file Member.h
 * @brief Déclaration de la classe Membre, base pour tous les membres du robot.
 *
 * Un Membre contient:
 *  - un ou plusieurs servos, chacun avec ses limites mécaniques et son état de position
 *  - une méthode setDestination() pour fixer un angle cible, avec contrainte aux limites
 *  - une méthode move() à appeler dans loop() pour faire avancer chaque servo vers sa
 *   destination, avec gestion du mode limiteur et du retour visuel LED.
 *
 * @note Un Membre gère un ou plusieurs servos chaînés, avec interpolation de position
 *      et retour visuel via LED. Chaque servo a des limites mécaniques définies
 *      par un ServoSpec.
 */

#pragma once

#include "ServoSpec.h"
#include <Meccanoid.h>
#include "../config/RobotConfig.h"


/**
 * @brief Identifiant de type pour les différents membres
 *
 * Permet un dispatch sécurisé sans RTTI (dynamic_cast) dans les fonctions de contrôle génériques. ( Non disponible sur Arduino Uno)
 * Chaque sous-classe override getType() pour retourner son type spécifique, et les fonctions de contrôle peuvent vérifier ce type avant de caster.
 */
enum class MemberType {GENERYC, EYES};

/** Nombre max de servos qu'un membre peut gérer (défini dans RobotConfig) */
#define MAX_SERVOS SERVOS_MAX_COUNT

// --- CLASSE DE BASE ---
/**
 * @brief Classe de base pour tous les membres du robot.
 *
 * Gère l'interpolation de position et le retour visuel LED
 * pour un ou plusieurs servos Meccanoid chaînés.
 */
class Member {

public:
    /** @brief Retourne le type du membre.
     * Override dans les sous-classes pour activer le dispatch sécurisé.
     *
     * @return L'id du type de ce membre (par défaut : GENERYC).
     */
    virtual ~Member() = default;
    virtual MemberType getType() const {return MemberType::GENERYC; };
    Member(int pin, const char* name, const ServoSpec* specs, uint8_t count);

    const char*         getName() const;
    void                setDestination(uint8_t index, int angle);
    virtual void        move();

    // Initialisation du membre : doit être appelée dans setup() pour configurer les servos et LEDs
    void begin(uint8_t cycles = 20);

protected:
    int         pin;
    const char* name;
    Chain       chain;
    uint8_t     servoCount;
    int         speed = SERVO_DEFAULT_SPEED;

    // Specs mécaniques (min, max, position, destination)
    struct ServoState {
        int min;
        int max;
        int position    = 90;
        int destination = 90;
        bool wasMoving = false;
    };
    ServoState states[MAX_SERVOS];

private:
    void updateLED(uint8_t index, bool moving);
};