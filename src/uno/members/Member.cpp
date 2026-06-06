/**
* @file   Member.cpp
 * @brief  Implémentation de la classe de base Membre.
 *
 * Gère l'interpolation de position et le retour visuel LED
 * pour un ou plusieurs servos Meccanoid chaînés.
 */

#include "Member.h"


// --- CONSTRUCTEUR ---
/**
 * @brief Initialise un membre avec ses servos et leurs limites mécaniques.
 *
 * Copie les limites de chaque ServoSpec dans l'état interne ; la position
 * et la destination de départ sont fixes à 90° (position neutre).
 *
 * @param pin Numéro de pin pour la communication avec les servos.
 * @param name Nom du membre (pour les logs).
 * @param specs Tableau de spécifications pour chaque servo (min, max).
 * @param count Nombre de servos dans ce membre (doit être ≤ MAX_SERVOS).
 */
Membre::Membre(
    int                 pin,    // Pin de communication pour les servos
    const char*         name,   // Nom du membre pour les logs
    const ServoSpec* specs,
    uint8_t             count   // Nombre de servos dans ce membre
    ): pin(pin), name(name), chain(pin), servoCount(count)
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
// --- MÉTHODES PUBLIQUES ---
/**@brief Retourne le nom du membre*/
const char* Membre::getName() const {
    return name;
}


/**
 * @brief Définit l'angle cible d'un servo, en respectant ses limites mécaniques.
 *
 * Si l'angle fourni dépasse les bornes [min, max], il est contraint (clamped)
 * et un avertissement est émis sur le port série.
 *
 * @param index Index du servo dans ce membre (0-based).
 * @param angle Angle souhaité, en degrés.
 *
 * @note Sans effet si index est hors limites.
 */
void Membre::setDestination(
    const uint8_t index,
    const int     angle) {
    if (index >= servoCount) return;

    // Contraindre l'angle aux limites mécaniques du servo
    // (clamping) et avertir si une contrainte a été appliquée (angle maximun que le servo peut atteindre)
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


/**
 * @brief Interpole chaque servo d'un pas vers sa destination.
 *
 * Doit être appelé à chaque itération de loop(). Active/désactive
 * le mode limiteur et la LED en fonction de l'état de mouvement.
 *
 * @note La vitesse d'avance est définie par le membre vitesse
 *       (valeur par défaut : SERVO_DEFAULT_SPEED).
 */
void Membre::move() {
    for (uint8_t i = 0; i < servoCount; i++) {
        ServoState& s = states[i];

        if (s.position == s.destination) {
            // Transition vers le repos : on active le mode limiteur et on éteint la LED
            if (s.wasMoving) {
                chain.getServo(i).setLim(true);
                updateLED(i, false);
                s.wasMoving = false;

                Serial.print(F("["));
                Serial.print(name);
                Serial.print(F("] Servo "));
                Serial.print(i);
                Serial.println(F(" → cible atteinte, passage au repos"));
            }
            continue;
        }

        // Premier tick de mouvement : on désactive le mode limiteur et on allume la LED
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

void Membre::begin(uint8_t cycles) {
    Serial.print(F("["));
    Serial.print(name);
    Serial.print(F("] Synchronisation de la chaîne ("));
    Serial.print(cycles);
    Serial.println(F(" cycles)…"));

    for (uint8_t i = 0; i < cycles; i++) {
        chain.update();
    }

    Serial.print(F("["));
    Serial.print(name);
    Serial.println(F("] Chaîne prête ✓"));
}

/**
 * @brief Met à jour la LED du servo en fonction de son état.
 *
 * @param index  Index du servo (et de sa LED dans la chaîne).
 * @param moving true → LED rouge (en mouvement), false → LED verte (repos).
 *
 * @warning La LED est associée au servo par son index dans la chaîne;
 *          une chaîne mal configurée peut allumer la mauvaise LED.
 */
void Membre::updateLED(uint8_t index, bool moving) {
    MeccanoServo servo = chain.getServo(index);
    if (moving) {
        servo.setColor(1, 0, 0); // Rouge
    } else {
        servo.setColor(0, 1, 0); // Vert
    }
}