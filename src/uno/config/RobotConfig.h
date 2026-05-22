/**
* @file RobotConfig.h
 * @brief Constantes globales partagées par tous les modules Arduino.
 */
#pragma once

#define DEBUG_MODE true // Activer ou désactiver les messages de debug dans le moniteur série

#define MAX_MEMBERS_COUNT 10 // Nombre maximum de membres enregistrable dans le robot (tête, bras, jambes, etc.)

// --- Communication série ---
#define SERIAL_BAUD_MONITOR 9600 // Baudrate utilisé pour le moniteur série (debug USB/PC)
#define SERIAL_BAUD_ESP32   9600 //// Baudrate utilisé pour la communication avec l'ESP32 (via Serial2 côté ESP)

// --- Servos ---
#define SERVO_DEFAULT_SPEED 5 // Vitesse d'interpolation par défaut (degrés par tick de loop)
#define SERVOS_MAX_COUNT 3 // Nombre maximum de servos par membre