// File: main.cpp
// Role : Point d'entrée principal du programme, gère l'initialisation et la boucle
// Description : Ce fichier contient le point d'entrée principal du programme, qui gère l'initialisation du robot et la boucle principale pour le contrôle du robot.
// Il inclut les fichiers de configuration et de communication nécessaires pour le fonctionnement du robot.

#include "members/head/Head.h"
#include "members/arm/Arm.h"
#include "communication/meccanoid/MeccanoidController.h"

Tete tete(6);
Bras brasGauche(9, "left");
Bras brasDroit(8, "right");

MeccanoidController controller;

void setup() {
    Serial.begin(9600);

    controller.add_member(&tete);
    controller.add_member(&brasGauche);
    controller.add_member(&brasDroit);
}

void loop() {
    controller.update();

    tete.move();
    brasGauche.move();
    brasDroit.move();

    // Exemple : faire bouger la caméra
    // Décommenter pour tester :
    // camera.setDestination(0, 45);   // Vers la gauche
    // delay(500);
    // camera.setDestination(0, 135);  // Vers la droite
    // delay(500);
}