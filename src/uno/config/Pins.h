// File: Pins.h
// Role : Définition des pins utilisés pour les servos, LED et moteur
// Description : Ce fichier contient les définitions des pins utilisés pour les servos, LED et moteur du robot
#pragma once

// Head
/*
 * SERVO_HEAD_1 : Pin pour le servo de la tête 1 (rotation horizontale)
 * SERVO_HEAD_2 : Pin pour le servo de la tête 2 (rotation verticale)
 */
#define SERVO_HEAD_1 2
#define SERVO_HEAD_2 3

// Left arm
/*
 * SERVO_ARM_L1 : Pin pour le servo du bras gauche 1 (épaule)
 * SERVO_ARM_L2 : Pin pour le servo du bras gauche 2 (sous-bras)
 * SERVO_ARM_L3 : Pin pour le servo du bras gauche 3 (coudes)
 */
#define SERVO_ARM_L1 4
#define SERVO_ARM_L2 5
#define SERVO_ARM_L3 6

// Right arm
/*
 * SERVO_ARM_R1 : Pin pour le servo du bras droit 1 (épaule)
 * SERVO_ARM_R2 : Pin pour le servo du bras droit 2 (sous-bras)
 * SERVO_ARM_R3 : Pin pour le servo du bras droit 3 (coudes)
 */
#define SERVO_ARM_R1 7
#define SERVO_ARM_R2 8
#define SERVO_ARM_R3 9

// LED & motor
/*
 * HEAD_LED_PIN : Pin pour la LED de la tête
 * FOOTER_MOTOR_PIN : Pin pour le moteur des pieds
 */
#define HEAD_LED_PIN 10
#define FOOTER_MOTOR_PIN 11
