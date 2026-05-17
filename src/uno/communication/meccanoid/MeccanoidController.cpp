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
    if (!SerialProtocol::read(cmd)) return;

    // Recherche du membre correspondant au nom de la commande
    for (uint8_t i = 0; i < memberCount; i++) {
        // Vérifier si le nom du membre correspond à la commande
        if (strcmp(membres[i]->getName(), cmd.member) != 0) continue;

        // --- Commande LED (ex: "led red") ---
        if (cmd.action[0] != '\0') {
            // Cast sûr : on sait que le membre "eyes" est une instance de Yeux
            Yeux* yeux = static_cast<Yeux*>(membres[i]);
            dispatchEyes(yeux, cmd.action);
        }
        // --- Commande servo (ex: "head 0 120") ---
        else {
            membres[i]->setDestination(cmd.servoIndex, cmd.angle);
        }

        break; // membre trouvé et commandé, sortir de la boucle
    }
}

/**
 * @brief Traduit une action textuelle en appel de méthode sur Yeux.
 *
 * @param yeux   Pointeur vers le membre yeux.
 * @param action Nom de l'action : "off", "white", "red", "green", "blue".
 */
void MeccanoidController::dispatchEyes(Yeux* yeux, const char* action) {
    if      (strcmp(action, "off")   == 0) yeux->eteindre();
    else if (strcmp(action, "white") == 0) yeux->setBlanc();
    else if (strcmp(action, "red")   == 0) yeux->setRouge();
    else if (strcmp(action, "green") == 0) yeux->setVert();
    else if (strcmp(action, "blue")  == 0) yeux->setBleu();
    else {
        Serial.print(F("[Controller] Action LED inconnue : "));
        Serial.println(action);
        Serial.println(F("'"));
    }
}
