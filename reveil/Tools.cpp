#include <EEPROM.h>      // Bibliothèque pour la mémoire interne de l'Arduino

/* ----------------------------------------------------------- */
/*                          EEPROM                             */
/* ----------------------------------------------------------- */
// Effacement de l'EEEPROM
void eeprom_clear() {
  for (int i = 0; i < 256; i++) {
    EEPROM.write(i, 0);
  }
}

// Recupération d'une valeur stockée
int eeprom_get(int add) {
  return (int) EEPROM.read(add);
}

// Stockage du valeur
void eeprom_set(int add, int val) {
  EEPROM.write(add, val);
}
