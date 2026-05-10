#pragma once
#include <MeccanoidServo.h>

struct ServoSpec {
    MeccanoidServo* servo;  // servo réel (fourni par Meccanoid)
    int min;                // limite min autorisée
    int max;                // limite max autorisée
    int position;           // position actuelle (logique)
    int destination;        // angle cible
};
