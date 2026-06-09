/**
 * @file MeccanoidController.cpp
 * @brief Implémentation de la classe MeccanoidController pour gérer les membres et traiter les commandes.
 *
 * Cette classe permet d'ajouter des membres (Membre) et de mettre à jour leurs positions en fonction
 * des commandes reçues via le protocole série.
 */

#include "MeccanoidController.h"
#include <string.h>

void MeccanoidController::addMember(Member* m) {
    if (memberCount >= MAX_MEMBERS) {
        Serial.print(F("[Controller] ERREUR : nombre maximum de membres atteint ("));
        Serial.print(MAX_MEMBERS);
        Serial.println(F(")"));
        return;
    }
    
    m->begin();
    members[memberCount++] = m;

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
        Serial.println(members[i]->getName());

        if (strcmp(members[i]->getName(), cmd.member) != 0) continue;

        Serial.println(F("[update] membre trouvé !"));

        if (cmd.action[0] != '\0') {
            Serial.println(F("[update] → branche action"));
            if (members[i]->getType() == MemberType::EYES) {
                Serial.println(F("[update] → dispatch eyes"));
                dispatchEyes(static_cast<Eyes *>(members[i]), cmd.action);
            }
        } else {
            Serial.println(F("[update] → branche servo"));
            members[i]->setDestination(cmd.servoIndex, cmd.angle);
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
void MeccanoidController::dispatchEyes(Eyes* yeux, const char* action) {
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

    if      (strcmp(action, "off")   == 0) yeux->turnOff();
    else if (strcmp(action, "white") == 0) yeux->setWhite();
    else if (strcmp(action, "red")   == 0) yeux->setRed();
    else if (strcmp(action, "green") == 0) yeux->setGreen();
    else if (strcmp(action, "blue")  == 0) yeux->setBlue();
    else {
        Serial.print(F("[Controller] Action LED inconnue : "));
        Serial.println(action);
    }
}
