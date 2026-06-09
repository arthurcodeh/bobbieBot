/**
 * @file Arm.cpp
 * @brief Implémentation de la classe Bras, représentant un bras du robot avec ses spécifications de servos.
 *
 * Cette classe hérite de Membre et initialise les spécifications des servos en fonction du côté (gauche ou droit).
 * Les spécifications des servos sont définies dans des tableaux statiques pour chaque côté
 * et sont utilisées pour configurer les servos lors de l'initialisation du membre.
 * Les spécifications des servos incluent les limites mécaniques (min et max), la position actuelle et la destination souhaitée.
 *
 * @param pin Le numéro de pin pour la communication avec les servos du bras
 * @param side Le côté du bras ("left" ou "right") pour déterminer les spéc
 *
 * @see Arm.h
*/
#include "Arm.h"

ServoSpec Arm::leftSpecs[LEFT_SERVO_COUNT] = {
    { 10, 170, 90, 90 },
    { 10, 180, 90, 90 },
    {  0,  90, 90, 90 }
};

ServoSpec Arm::rightSpecs[RIGHT_SERVO_COUNT] = {
    { 10, 170, 90, 90 },
    { 10, 180, 90, 90 },
    { 90, 180, 90, 90 }
};