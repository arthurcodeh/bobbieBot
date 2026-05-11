// File: main.cpp
// Role : Point d'entrée principal du programme Arduino
// Description : Initialise les membres du robot et orchestre la boucle principale.

#include "members/head/Head.h"
#include "members/arm/Arm.h"
#include "communication/meccanoid/MeccanoidController.h"
#include "config/Pins.h"
#include "config/RobotConfig.h"

// --- Membres du robot ---
Tete tete(PIN_HEAD);
Bras brasGauche(PIN_ARM_LEFT,  "left");
Bras brasDroit(PIN_ARM_RIGHT, "right");

// --- Contrôleur central (reçoit et dispatche les commandes série) ---
MeccanoidController controller;

void setup() {
    // ✅ Serial.begin() appelé EN PREMIER, avant tout le reste
    Serial.begin(SERIAL_BAUD_MONITOR);
    delay(1000); // laisse le temps au moniteur série de se connecter

    Serial.println(F("[main] Arduino démarré ✓"));

    // Enregistrement des membres auprès du contrôleur
    controller.add_member(&tete);
    controller.add_member(&brasGauche);
    controller.add_member(&brasDroit);

    Serial.println(F("[main] Membres enregistrés ✓"));

    // Position initiale de test
    tete.setDestination(0, 45);
}

void loop() {
    // Lecture et dispatch des commandes reçues depuis l'ESP32
    controller.update();

    // Interpolation des servos vers leurs destinations respectives
    tete.move();
    brasGauche.move();
    brasDroit.move();
}