/**
 * @file MeccanoidController.cpp
 * @brief Implémentation de la classe MeccanoidController pour gérer les membres et traiter les commandes.
 *
 * Cette classe permet d'ajouter des membres (Membre) et de mettre à jour leurs positions en fonction
 * des commandes reçues via le protocole série.
 */

#include "MeccanoidController.h"
#include <string.h>

void MeccanoidController::add_member(Membre* m) {
    if (memberCount >= MAX_MEMBERS) {
        Serial.print(F("[Controller] ERREUR : nombre maximum de membres atteint ("));
        Serial.print(MAX_MEMBERS);
        Serial.println(F(")"));
        return;
    }
    
    m->begin();
    membres[memberCount++] = m;

    if (DEBUG_MODE) {
        Serial.print(F("[Controller] Membre ajouté : "));
        Serial.println(m->getName());
        Serial.println(F("'"));
    }
}

void MeccanoidController::update() {
    Command cmd;

    if (!SerialProtocol::read(cmd)) return;

    if (DEBUG_MODE) {
        Serial.print(F("[update] member='"));
        Serial.print(cmd.member);
        Serial.print(F("' action='"));
        Serial.print(cmd.action);
        Serial.print(F("' action[0]="));
        Serial.println((int)cmd.action[0]);
    }


    for (uint8_t i = 0; i < memberCount; i++) {
        Serial.print(F("[update] comparaison avec : "));
        Serial.println(membres[i]->getName());

        if (strcmp(membres[i]->getName(), cmd.member) != 0) continue;

        Serial.println(F("[update] membre trouvé !"));

        if (cmd.action[0] != '\0') {
            Serial.println(F("[update] → branche action"));
            if (membres[i]->getType() == MemberType::EYES) {
                Serial.println(F("[update] → dispatch eyes"));
                dispatchEyes(static_cast<Yeux*>(membres[i]), cmd.action);
            }
        } else {
            Serial.println(F("[update] → branche servo"));
            membres[i]->setDestination(cmd.servoIndex, cmd.angle);
        }

        break;
    }
}

/**
 * @brief Traduit une action textuelle en appel de méthode sur Yeux.
 *
 * @param yeux   Pointeur vers le membre yeux.
 * @param action Nom de l'action: "off", "white", "red", "green", "blue".
 */
void MeccanoidController::dispatchEyes(Yeux* yeux, const char* action) {
    if (DEBUG_MODE) {
        Serial.print(F("[dispatchEyes] len="));
        Serial.print(strlen(action));
        Serial.print(F(" bytes : "));
        for (uint8_t i = 0; i < strlen(action); i++) {
            Serial.print((int)action[i]);
            Serial.print(' ');
        }
        Serial.println();
    }

    if      (strcmp(action, "off")   == 0) yeux->eteindre();
    else if (strcmp(action, "white") == 0) yeux->setBlanc();
    else if (strcmp(action, "red")   == 0) yeux->setRouge();
    else if (strcmp(action, "green") == 0) yeux->setVert();
    else if (strcmp(action, "blue")  == 0) yeux->setBleu();
    else {
        Serial.print(F("[Controller] Action LED inconnue : "));
        Serial.println(action);
    }
}
