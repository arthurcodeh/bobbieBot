/**
 * @file Head.cpp
 * @brief Implémentation de la classe Tete, représentant la tête du robot.
 *
 * La classe Tete hérite de Membre, qui gère la communication avec les servos et les limites mécaniques.
 * Tete fournit des méthodes spécifiques pour contrôler les mouvements de la tête de manière plus intuitive
 * (ex: tournerGauche(30) au lieu de setDestination(0,30)).
 *
 * Servo 0 — rotation horizontale (gauche / droite) : limites [10°, 170°]
 * Servo 1 — inclinaison verticale (haut / bas)     : limites [45°, 135°]
 * @see Membre
 * @see ServoSpec
 * @see Head.h
 */

#include "Head.h"

//  servo*   min   max   position  destination
ServoSpec Tete::headSpecs[HEAD_SERVO_COUNT] = {
    { 10, 170, 90, 90 },    // servo 0 : rotation horizontale
    //{45,  135,   90,   90 },   // servo 1 : inclinaison verticale
};
