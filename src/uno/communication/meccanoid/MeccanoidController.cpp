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
            break; // Commande traitée, sortir de la boucle
        }
    }
}
