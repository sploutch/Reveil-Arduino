/*!
* -----------------------------------------------------------
*                           EEPROM
* -----------------------------------------------------------
* @file Tools.h
*
* Written by Sploutch
*
* BSD license, all text here must be included in any redistribution.
*
*/

#ifndef TOOLS
#define TOOLS

#include <EEPROM.h> // Bibliothèque pour la mémoire interne de l'Arduino

// Effacement de l'EEEPROM
void eepromClear();

// Recupération d'une valeur stockée
int eepromGet(int add);

// Stockage du valeur
void eepromSet(int add, int val);

#endif