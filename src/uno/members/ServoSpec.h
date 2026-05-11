// File: ServoSpec.h
// Role: Décrit les caractéristiques et l'état d'un servo individuel
#pragma once

// Limites mécaniques et état d'un servo
struct ServoSpec {
    int min;
    int max;
    int position    = 90;
    int destination = 90;

    // Constructeur pour l'initialisation avec { min, max, pos, dest }
    ServoSpec(int min, int max, int position = 90, int destination = 90)
        : min(min), max(max), position(position), destination(destination) {}
};
