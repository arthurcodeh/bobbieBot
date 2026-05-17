/**
 * @file SerialProtocol.h
 * @brief Déclaration de la classe SerialProtocol pour parser les commandes série.
 *
 * Cette classe lit les lignes de texte envoyées par l'ESP32 via le port série,
 * les parse en commandes structurées (Command) et fournit une interface simple
 * pour vérifier la validité des commandes et accéder à leurs paramètres.
 *
 * Protocole de commande :
 *   - Commande servo : "head 0 120" → membre "head", servo index 0, angle 120°
 *   - Commande LED : "led red" → LED rouge / "led off" → LED éteinte
 *
 *   Protocole série utilisés :
 *   - Serial -> Debug USB (monitor série PC)
 *   - SoftwareSerial -> Communication avec l'ESP32 (RX/TX) (Uno)
 *   - Serial1 -> Communication avec l'ESP32 (RX/TX) (Mega)
 *
 * Chaque commande doit être terminée par un saut de ligne '\n' (envoyé par println).
 */

#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../../config/Pins.h"
#include "../../config/RobotConfig.h"

/**
 * @brief Commande parsée, prête à être exécutée par le contrôleur.
 *
 * Deux types de commandes existent :
 *  - Servo  : member + servoIndex + angle   (action est vide)
 *  - Action : member + action               (servoIndex et angle sont à -1)
 */
struct Command {
    char member[16];   // nom du membre ciblé : "head", "led", ...
    char action[16];   // pour les LED : "red", "off", ... / vide pour les servos
    int  servoIndex;   // index du servo (ignoré si c'est une commande LED)
    int  angle;        // angle cible en degrés (ignoré si c'est une commande LED)
    bool valid;        // false si le parsing a échoué
};

class SerialProtocol {
public:
    /**
     * @brief Initialise le SoftwareSerial vers l'ESP32 et le port debug.
     *        Doit être appelé dans setup().
     */
    static void begin();

    /**
     * @brief Lit et parse une commande disponible sur Serial ou SoftwareSerial.
     *        Doit être appelé dans loop().
     *
     * @param out Référence vers la Command à remplir si une ligne est disponible.
     * @return true si une commande valide a été reçue et parsée, false sinon.
     */
    static bool read(Command& out);

    /**
    * @brief Instance SoftwareSerial pour la communication avec l'ESP32.
    *        Exposée publiquement pour permettre une réinitialisation externe si nécessaire.
    */
    static SoftwareSerial espSerial;

private:
    /**
     * @brief Parse une ligne brute en struct Command.
     *
     * Détecte automatiquement le type de commande :
     *  - Deux tokens  ("eyes red")      → commande action
     *  - Trois tokens ("head 0 120")    → commande servo
     *
     * @param line Ligne reçue, déjà trimmée.
     * @return Command parsée (cmd.valid = false si mal formée).
     */
    static Command parse(const String& line);
};



