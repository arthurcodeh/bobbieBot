/**
 * @file MeccanoidController.h
 * @brief Déclaration de la classe MeccanoidController pour gérer les membres et traiter les
 * commandes reçues via le protocole série.
 *
 * MeccanoidController permet d'ajouter des membres (instances de Membre) et de mettre à jour
 * leurs positions en fonction des commandes reçues de l'ESP32. Chaque membre est identifié
 * par un nom unique, et les commandes sont acheminées vers le membre correspondant pour ajuster
 * les angles de ses servos.
 *
 * Le protocole de commande attendu est du type :
 *  "head 0 120"  → membre "head", servo index 0
 *  "led red"     → commande LED pour le membre "led"
 *  "led off"     → commande LED pour le membre "led"
 *
 *      Chaque commande est terminée par un saut de ligne '\n' envoyé par l'ESP32.
 */

#pragma once

#include "../../members/Member.h"
#include "../../members/head/eyes/eyes.h"
#include "../serial/SerialProtocol.h"
#include "../../config/RobotConfig.h"

#define MAX_MEMBERS MAX_MEMBERS_COUNT

class MeccanoidController {

public:
    /**
    * @brief Enregistre un membre dans le contrôleur.
    *        Doit être appelé dans setup() pour chaque membre actif.
    *
    * @param m Pointeur vers le membre à enregistrer.
    */
    void add_member(Membre* m);

    /**
     * @brief Lit les commandes disponibles et les dispatche vers les membres.
     *        Doit être appelé à chaque itération de loop().
     */
    void update();

private:
    Membre* membres[MAX_MEMBERS];
    uint8_t memberCount = 0;

    /**
     * @brief Traduit une action textuelle en appel de méthode sur Yeux.
     *
     * @param yeux   Pointeur vers le membre yeux.
     * @param action Nom de l'action : "off", "white", "red", "green", "blue".
     */
    void dispatchEyes(Yeux* yeux, const char* action);
};
