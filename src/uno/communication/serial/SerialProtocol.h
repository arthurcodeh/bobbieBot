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
 * Chaque commande doit être terminée par un saut de ligne '\n' (envoyé par println).
 */

#pragma once

#include <Arduino.h>
 
// Représente une commande parsée, prête à être exécutée
struct Command {
    char member[16];   // nom du membre ciblé : "head", "led", ...
    char action[16];   // pour les LED : "red", "off", ... / vide pour les servos
    int  servoIndex;   // index du servo (ignoré si c'est une commande LED)
    int  angle;        // angle cible en degrés (ignoré si c'est une commande LED)
    bool valid;        // false si le parsing a échoué
};
 
class SerialProtocol {
public:
    // Doit être appelé dans setup() — initialise le port série
    static void begin(unsigned long baudRate);
 
    // Doit être appelé dans loop() — retourne true si une commande est disponible
    // Si true, la commande est stockée dans 'out'
    static bool read(Command& out);

private:
    // Parse une ligne brute en Command
    static Command parse(const String& line);
};
 


