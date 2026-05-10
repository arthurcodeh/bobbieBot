// File: ServoSpec.h
// Role: Décrit les caractéristiques et l'état d'un servo individuel
#pragma once
#include <MeccanoidServo.h>

struct ServoSpec {
    MeccanoidServo* servo;  // servo réel (fourni par Meccanoid "le servo physique")
    int min;                // limite min autorisée
    int max;                // limite max autorisée
    int position;           // position actuelle (logique)
    int destination;        // angle cible
};
