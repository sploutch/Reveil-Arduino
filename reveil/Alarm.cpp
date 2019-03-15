/*!
* -----------------------------------------------------------
*                           ALARM
* -----------------------------------------------------------
* @file Alarm.cpp
*
* Written by Sploutch
*
* BSD license, all text here must be included in any redistribution.
*
*/

#include "Alarm.h"

Alarm::Alarm(Afficheur& affich_ref) : afficheur_g(affich_ref){}

// Auto vérification de l'arame : ON, OFF, Intensité
void Alarm::autoCheck() {
	// ON
	if (this->isActivate == true) {
		// Pas encore en route
		if (this->isRunning == false) {
			if (this->checkIfAlarmNeedStart() == true) {
				this->startAlarm();
				this->calcul_duree_running();
			}

			// Déja en route
		} else {
			this->calcul_duree_running();
			this->augmenterIntensite();
			this->checkIfAlarmNeedStop();
		}
	}
}

// Calcul de la durée de sonnerie
void Alarm::calcul_duree_running() {
	if (this->isRunning == false) {
		return;
	}

	// Avant minuit
	if (this->afficheur_g.alarm_hh <= this->afficheur_g.current_hh) {
		this->hh_running = this->afficheur_g.current_hh - this->afficheur_g.alarm_hh;
	} else {
		this->hh_running = (this->afficheur_g.current_hh + 24) - this->afficheur_g.alarm_hh;
	}

	// L'heure est pas passée
	if (this->afficheur_g.alarm_mm <= this->afficheur_g.current_mm) {
		this->mm_running = this->afficheur_g.current_mm - this->afficheur_g.alarm_mm;
	} else {
		this->mm_running = (this->afficheur_g.current_mm + 60) - this->afficheur_g.alarm_mm;
		this->hh_running--;
	}

	// La minute est passée
	if (this->afficheur_g.alarm_ss <= this->afficheur_g.current_ss) {
		this->ss_running = this->afficheur_g.current_ss - this->afficheur_g.alarm_ss;
	} else {
		this->ss_running = (this->afficheur_g.current_ss + 60) - this->afficheur_g.alarm_ss;
		this->mm_running--;
	}

	int ss = (this->hh_running * 3600) + (this->mm_running * 60) + this->ss_running;
	if(ss != this->sum_ss_running){
		this->sum_ss_running = ss;  
		Serial.print("Alarm running : ");
		Serial.print(this->sum_ss_running);
		Serial.print("sec\n");
	}
}

// Controle si l'alarme doit être demarrée
bool Alarm::checkIfAlarmNeedStart() {
	if (this->isActivate == false) {
		return false;
	}
	if (this->afficheur_g.alarm_hh == this->afficheur_g.current_hh && this->afficheur_g.alarm_mm == this->afficheur_g.current_mm && this->afficheur_g.alarm_ss == this->afficheur_g.current_ss) {
		return true;
	}
	return false;
}

// Controle si l'alarme doit s'arréter
void Alarm::checkIfAlarmNeedStop() {
	if (this->isRunning == true) {
		if (this->sum_ss_running >= ALARM_NB_SEC_TO_STOP) {
			Serial.print("Alarme Need Stop\n");
			this->stopAlarm();
		}
	}
}

// Arrete de l'alarme
void Alarm::stopAlarm() {
	Serial.print("Alarme Stop\n");
	analogWrite(PIN_LED_SUN, 0);

	this->isRunning = false;
	this->hh_running = 0;
	this->mm_running = 0;
	this->ss_running = 0;
	this->sum_ss_running = 0;

	this->afficheur_g.displayAlameSTOP();
	delay(DELAY_SHOW_MSG_STOP);
}

// Démarage de l'alarme
void Alarm::startAlarm() {
	Serial.print("Alarme Start\n");
	this->isRunning = true;
	this->intensity = 1;
	this->augmenterIntensite();
}

// Augmentation de l'intensité de la lumiére
void Alarm::augmenterIntensite() {
	if (this->intensity < 255 && this->isRunning == true) {
		float new_val = (float)255.00 * ((float)(this->sum_ss_running + 1) / (float)(ALARM_NB_SEC_TO_INCREMENT * 255));
		if (new_val > 255.00) {
			new_val = 255.00;
		}
		if (new_val <= 0.00) {
			new_val = 1.00;
		}

		if ((int)new_val != this->intensity) {
			this->intensity = (int)new_val;
			analogWrite(PIN_LED_SUN, this->intensity);
			Serial.print("Intensity set to : ");
			Serial.print(this->intensity);
			Serial.print("\n");
		}
	}
}