/*!
* -----------------------------------------------------------
*                           TOOLS
* -----------------------------------------------------------
* @file Tools.cpp
*
* Written by Sploutch
*
* BSD license, all text here must be included in any redistribution.
*
*/

#include "Tools.h"

/** Effacement de l'EEEPROM **/
void eepromClear() {
	for (int i = 0; i < 256; i++) {
		EEPROM.write(i, 0);
	}
}

/** Recupération d'une valeur stockée **/
int eepromGet(int add) {
	return (int) EEPROM.read(add);
}

/** Stockage du valeur **/
void eepromSet(int add, int val) {
	EEPROM.write(add, val);
}
