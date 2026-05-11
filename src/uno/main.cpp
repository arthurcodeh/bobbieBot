#include "members/head/Head.h"
#include "members/arm/Arm.h"
#include "communication/meccanoid/MeccanoidController.h"
#include "config/Pins.h"
#include "config/RobotConfig.h"
#include "Meccanoid.h"

// ✅ Pointeurs — pas encore construits
Tete* tete         = nullptr;
Bras* brasGauche   = nullptr;
Bras* brasDroit    = nullptr;

MeccanoidController controller;

void setup() {
    Serial.begin(SERIAL_BAUD_MONITOR);
    delay(2000);

    Serial.println(F("[main] Arduino démarré ✓"));

    tete       = new Tete(PIN_HEAD);
    //brasGauche = new Bras(PIN_ARM_LEFT,  "left");
    //brasDroit = new Bras(PIN_ARM_RIGHT, "right");

    controller.add_member(tete);
    //controller.add_member(brasGauche);
    //controller.add_member(brasDroit);
    // Test : envoie la position 45° à chaque ID possible


    Serial.println(F("[main] Membres enregistrés ✓"));

    tete->setDestination(0, 45);

}

void loop() {
    controller.update();

    tete->move();
    //brasGauche->move();
    //brasDroit->move();
}