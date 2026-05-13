// File: Head.h
// Role: Représente la tête du robot (2 servos)
//
// Servo 0 — rotation horizontale (gauche / droite) : limites [10°, 170°]
// Servo 1 — inclinaison verticale (haut / bas)     : limites [45°, 135°]

/**
 * @file Head.h
 * @brief Représente la tête du robot (2 servos)
 *
 * Servo 0 — rotation horizontale (gauche / droite) : limites [10°, 170°]
 * Servo 1 — inclinaison verticale (haut / bas)     : limites [45°, 135°]
 *
 * La classe Tete hérite de Membre, qui gère la communication avec les servos et les limites mécaniques.
 * Tete fournit des méthodes spécifiques pour contrôler les mouvements de la tête de manière plus intuitive
 * (ex: tournerGauche(30) au lieu de setDestination(0, 30)).
 *
 * @param pin Le numéro de pin pour la communication avec les servos de la tête
 *
 * @see Membre
 * @see ServoSpec
 */

#pragma once
#include "../Member.h"
#include "uno/config/Pins.h"

static const uint8_t HEAD_SERVO_COUNT = SERVOS_HEAD;

class Tete : public Membre {
private:
    // Specs statiques des 2 servos de la tête
    // (statique = partagé par toutes les instances, stocké une seule fois en mémoire)
    static ServoSpec headSpecs[HEAD_SERVO_COUNT];

public:
    explicit Tete(int pin)
        : Membre(pin, "head", headSpecs, HEAD_SERVO_COUNT) {}

    // --- Raccourcis (simplifie les choses) ---
    // tournerGauche(30) => setDestination(0, 30)

    void tournerGauche(int angle)   { setDestination(0, angle); }
    void tournerDroite(int angle)   { setDestination(0, angle); }
    void inclinerHaut(int angle)    { setDestination(1, angle); }
    void inclinerBas(int angle)     { setDestination(1, angle); }
    void centrer()                  { setDestination(0, 90); setDestination(1, 90); }
};