/*!
* -----------------------------------------------------------
*                           BUTTON
* -----------------------------------------------------------
* @file Button.cpp
*
* Written by Sploutch
*
* BSD license, all text here must be included in any redistribution.
*
*/

#include "Button.h"

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
