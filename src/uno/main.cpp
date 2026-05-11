// File: main.cpp
// Role : Point d'entrée principal du programme, gère l'initialisation et la boucle
// Description : Ce fichier contient le point d'entrée principal du programme, qui gère l'initialisation du robot et la boucle principale pour le contrôle du robot.
// Il inclut les fichiers de configuration et de communication nécessaires pour le fonctionnement du robot.

#include "members/head/Head.h"
#include "members/arm/Arm.h"
#include "communication/meccanoid/MeccanoidController.h"
#include "config/Pins.h"
#include "config/RobotConfig.h"

Tete tete(PIN_HEAD);
Bras brasGauche(PIN_ARM_LEFT, "left");
Bras brasDroit(PIN_ARM_RIGHT, "right");

MeccanoidController controller;

void setup() {
    Serial.begin(SERIAL_BEGIN);
    delay(1000);
    Serial.println("[main] Arduino démarré ✓");

    controller.add_member(&tete);
    controller.add_member(&brasGauche);
    controller.add_member(&brasDroit);

    tete.setDestination(0, 45);
}

void loop() {
    controller.update();

    tete.move();
    brasGauche.move();
    brasDroit.move();

    tete.setDestination(0, 45);

    // Exemple : faire bouger la caméra
    // Décommenter pour tester :
    // camera.setDestination(0, 45);   // Vers la gauche
    // delay(500);
    // camera.setDestination(0, 135);  // Vers la droite
    // delay(500);
}