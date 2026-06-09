/**
 * @file eyes.h
 * @brief Représente les yeux du robot (2 LEDs Meccanoid).
 *
 * Led 0 — œil gauche
 * Led 1 — œil droit
 *
 * Yeux hérite de Membre mais ne gère pas de servos : le tableau de specs
 * est vide (nullptr, count = 0). Seules les LEDs de la chaîne sont utilisées.
 *
 * Les méthodes de raccourci (eteindre, setBlanc, etc.) appliquent la même
 * couleur aux deux yeux simultanément.
 *
 * @see Member.h
 */

#pragma once

#include "../../Member.h"
#include "uno/config/Pins.h"


class Eyes : public Member {
public:
    /**
     * @brief Identifie ce membre comme EYES pour un cast sécurisé.
     * @see MemberType
     */
    MemberType getType() const override { return MemberType::EYES; };

    /**
     * @brief Initialise les yeux sur le pin donné, sans servo associé.
     * @param pin Pin de communication avec la chaîne Meccanoid.
     */
    explicit Eyes(int pin)
        : Member(pin, "eyes", nullptr, 0) {}

    // --- Raccourcis de couleur ---
    // Chaque méthode applique la même couleur aux deux yeux (index 0 et 1).

    /** Éteint les deux yeux. */
    void turnOff() {
        chain.getLed(0).setColor(0, 0, 0, 0);
        chain.getLed(1).setColor(0, 0, 0, 0);
    }

    /** Allume les deux yeux en blanc. */
    void setWhite() {
        chain.getLed(0).setColor(7, 7, 7, 0);
        chain.getLed(1).setColor(7, 7, 7, 0);
    }

    /** Allume les deux yeux en rouge. */
    void setRed() {
        chain.getLed(0).setColor(7, 0, 0, 0);
        chain.getLed(1).setColor(7, 0, 0, 0);
    }

    /** Allume les deux yeux en vert. */
    void setGreen() {
        chain.getLed(0).setColor(0, 7, 0, 0);
        chain.getLed(1).setColor(0, 7, 0, 0);
    }

    /** Allume les deux yeux en bleu. */
    void setBlue() {
        chain.getLed(0).setColor(0, 0, 7, 0);
        chain.getLed(1).setColor(0, 0, 7, 0);
    }

    /**
     * @brief Applique une couleur personnalisée aux deux yeux.
     *
     * @param r Intensité rouge  (0–7)
     * @param g Intensité vert   (0–7)
     * @param b Intensité bleu   (0–7)
     * @param fadetime Durée du fondu (0 = instantané, voir Meccanoid.h pour les valeurs)
     */
    void setColor(byte r, byte g, byte b, byte fadetime = 0) {
        chain.getLed(0).setColor(r, g, b, fadetime);
        chain.getLed(1).setColor(r, g, b, fadetime);
    }

    void move() override {
        chain.update();
        chain.update();
    }
};