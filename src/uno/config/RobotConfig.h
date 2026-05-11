// File: RobotConfig.h
// Role : Configuration générale du robot
// Description : Constantes globales partagées par tous les modules du programme Arduino.

#pragma once

// --- Communication série ---
// Baudrate utilisé pour le moniteur série (debug USB)
#define SERIAL_BAUD_MONITOR 9600

// Baudrate utilisé pour la communication avec l'ESP32 (via Serial2 côté ESP)
#define SERIAL_BAUD_ESP32   115200

// --- Servos ---
// Vitesse d'interpolation par défaut (degrés par tick de loop)
#define SERVO_DEFAULT_SPEED 5