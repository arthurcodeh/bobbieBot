// File: Head.cpp
// Role: Définition des limites mécaniques des servos de la tête

#include "Head.h"

//  servo*   min   max   position  destination
ServoSpec Tete::headSpecs[HEAD_SERVO_COUNT] = {
    { 10, 170, 90, 90 },    // servo 0 : rotation horizontale
    //{45,  135,   90,   90 },   // servo 1 : inclinaison verticale
};

// Head 0 20