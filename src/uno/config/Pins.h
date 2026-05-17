/**
* @file Pins.h
 * @brief Définition des pins matériels du robot.
 *
 * Centralise toutes les assignations de pins pour faciliter
 * les modifications hardware sans toucher au reste du code.
 */
#pragma once

// Head
/*
 * PIN_HEAD : Pin pour les servos de la tête
 * SERVOS_HEAD : Nombre de servos utilisés pour la tête
 */
#define PIN_HEAD 6
#define SERVOS_HEAD 2


// Left arm
/*
 * PIN_ARM_LEFT : Pin pour les servos du bras gauche
 * SERVOS_ARM_LEFT : Nombre de servos utilisés pour le bras gauche
 */
#define PIN_ARM_LEFT 9
#define SERVOS_ARM_LEFT 3


// Right arm
/*
 * PIN_ARM_RIGHT : Pin pour les servos du bras droit
 * SERVOS_ARM_RIGHT : Nombre de servos utilisés pour le bras droit
 */
#define PIN_ARM_RIGHT 8
#define SERVOS_ARM_RIGHT 3


// LED & motor
/*
 * HEAD_LED_PIN : Pin pour la LED de la tête
 * FOOTER_MOTOR_PIN : Pin pour le moteur des pieds
 */
#define HEAD_LED_PIN 10
#define FOOTER_MOTOR_PIN 11

// Communication ESP32 (SoftwareSerial — Arduino Uno)
#define RX_ESP32 2   // Reçoit depuis TX de l'ESP32
#define TX_ESP32 3   // Émet vers RX de l'ESP32
