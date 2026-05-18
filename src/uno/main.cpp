/**
 * @file main.cpp
 * @brief Point d'entrée du programme pour Arduino Uno.
 *
 * Ce fichier initialise les composants du robot et gère la boucle principale.
 */

#include "members/head/Head.h"
#include "members/head/eyes/eyes.h"
#include "members/arm/Arm.h"
#include "communication/meccanoid/MeccanoidController.h"
#include "config/Pins.h"
#include "config/RobotConfig.h"

/**
 * Pointeurs vers les membres du robot. Initialisés dans setup() après la configuration de la communication.
 */
Tete* tete         = nullptr;
Yeux* yeux         = nullptr;
Bras* brasGauche   = nullptr;
Bras* brasDroit    = nullptr;

MeccanoidController controller;


/**
 * @brief Fonction d'initialisation appelée une fois au démarrage de l'Arduino.
 *
 * Cette fonction configure la communication série, initialise les membres du robot
 * et les enregistre auprès du contrôleur de communication.
 */
void setup() {
    Serial.begin(SERIAL_BAUD_MONITOR);
    delay(2000);

    Serial.println(F("[main] Arduino démarré ✓"));

    tete       = new Tete(PIN_HEAD);
    yeux       = new Yeux(HEAD_LED_PIN);
    //brasGauche = new Bras(PIN_ARM_LEFT,  "left");
    //brasDroit = new Bras(PIN_ARM_RIGHT, "right");

    tete->begin();
    yeux->begin();

    controller.add_member(tete);
    controller.add_member(yeux);
    //controller.add_member(brasGauche);
    //controller.add_member(brasDroit);

    Serial.println(F("[main] Membres enregistrés ✓"));
}


/**
 * @brief Boucle principale appelée en continu après setup().
 *
 * Cette fonction met à jour la communication avec le contrôleur et déplace les membres
 * vers leurs destinations respectives.
 */
void loop() {
    controller.update();

    tete->move();
    //brasGauche->move();
    //brasDroit->move();
}