// File: eyes.h
// Role: Représente les yeux du robot (2 leds)
//
// Led 0 — œil gauche
// Led 1 — œil droit

#pragma once
#include "../Member.h"
#include "uno/config/Pins.h"

class Yeux : public Membre {
    explicit Yeux(int pin)
        : Membre(pin, "eyes", nullptr, 0) {} // Pas de specs pour les LEDs


    // --- Raccourcis (simplifie les choses) ---

};