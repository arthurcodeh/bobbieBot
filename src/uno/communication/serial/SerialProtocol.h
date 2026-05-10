// File: SerialProtocol.h
// Role: Parser les commandes texte reçues depuis l'ESP32 via le port série
//
// Protocole ex :
//   "head 0 120"  → membre "head", servo index 0, angle 120°
//   "led red"     → LED couleur rouge
//   "led off"     → LED éteinte
//
// Chaque commande est terminée par un saut de ligne '\n'
 
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
 


