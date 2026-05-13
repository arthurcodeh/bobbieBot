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
    if (memberCount < MAX_MEMBERS) {
        membres[memberCount++] = m;
    }
}

void MeccanoidController::update() {
   Command cmd;

    // rien faire si aucune commande reçue
    if (!SerialProtocol::read(cmd)) {
        return;
    }

    // Traiter la commande reçue
    for (uint8_t i = 0; i < memberCount; i++) {
        if (strcmp(membres[i]->getName(), cmd.member) == 0) {
            membres[i]->setDestination(cmd.servoIndex, cmd.angle);
            break; // Commande traitée, sortir de la boucle
        }
    }
}
