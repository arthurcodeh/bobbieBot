/**
 * @file ServoSpec.h
 * @brief Spécification d'un servo : limites mécaniques, position actuelle et destination.
 *
 * Ce struct est utilisé pour stocker les informations de chaque servo, notamment les limites mécaniques (min et max),
 * la position actuelle et la destination souhaitée. Il peut être initialisé avec des valeurs par défaut ou personnalisées.
 */

#pragma once


struct ServoSpec {
    int min;
    int max;
    int position    = 90;
    int destination = 90;

    // Constructeur pour l'initialisation avec { min, max, pos, dest }
    ServoSpec(int min, int max, int position = 90, int destination = 90)
        : min(min), max(max), position(position), destination(destination) {}
};
