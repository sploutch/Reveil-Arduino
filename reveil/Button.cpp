#include "Button.h"


/* ----------------------------------------------------------- */
/*                         BUTTON                              */
/* ----------------------------------------------------------- */
// Configuration de l'entrée du bouton
void Button::setPin(int pin) {
  this->pin = pin;
  pinMode(this->pin, INPUT);
}

// Mise à jour de l'état du bouton
void Button::setEtat() {
  this->etat = digitalRead(this->pin);
}

// Mise à jour de l'état du bouton
void Button::setEtat(int etat) {
  this->etat = etat;
}

// Lecture de l'état du bouton
bool Button::readPinEtat() {
  return digitalRead(this->pin);
}

// Vérifie si le bouton est pressé ou non
bool Button::isPressed() {
  if (this->etat == HIGH) {
    return false;
  } else {
    return true;
  }
}
