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

    // rien faire si aucune commande reçue
    if (!SerialProtocol::read(cmd)) return;

    // Recherche du membre correspondant au nom de la commande
    for (uint8_t i = 0; i < memberCount; i++) {
        // Vérifier si le nom du membre correspond à la commande
        if (strcmp(membres[i]->getName(), cmd.member) != 0) continue;

        // --- Commande LED (ex: "led red") ---
        if (membres[i]->getType() != MemberType::EYES) {
            Serial.print(F("[Controller] ERREUR : '"));
            Serial.print(membres[i]->getName());
            Serial.println(F("' ne supporte pas les commandes action"));
            break;
        }
        Yeux* yeux = static_cast<Yeux*>(membres[i]); // sûr : type vérifié
        dispatchEyes(yeux, cmd.action);
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
