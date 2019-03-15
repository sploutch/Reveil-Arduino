/*!
* -----------------------------------------------------------
*                           BUTTONS
* -----------------------------------------------------------
* @file Buttons.cpp
*
* Written by Sploutch
*
* BSD license, all text here must be included in any redistribution.
*
*/

#include "Buttons.h"

// Init des bouttons
Buttons::Buttons(Afficheur& p_afficheur_g, Alarm& p_alarm_g) : p_afficheur_g(p_afficheur_g), p_alarm_g(p_alarm_g) {
	this->btn_alarm_conf.setPin(PIN_BTN_ALARM_CONF);
	this->btn_add_mm.setPin(PIN_BTN_ADD_MM);
	this->btn_add_hh.setPin(PIN_BTN_ADD_HH);
	this->btn_time_conf.setPin(PIN_BTN_TIME_CONF);
	this->btn_light.setPin(PIN_BTN_LIGHT);
	this->btn_switch_alarm.setPin(PIN_SWITCH_ALARM);

	this->btn_alarm_conf.setEtat();
	this->btn_add_mm.setEtat();
	this->btn_add_hh.setEtat();
	this->btn_time_conf.setEtat();
	this->btn_light.setEtat();
	this->btn_switch_alarm.setEtat();
}

// Check de l'état de tous les bouttons
void Buttons::checkAllButtonsAction() {
	this->checkButtonAction(this->btn_alarm_conf);
	this->checkButtonAction(this->btn_add_mm);
	this->checkButtonAction(this->btn_add_hh);
	this->checkButtonAction(this->btn_time_conf);
	this->checkButtonAction(this->btn_light);
	this->checkButtonAction(this->btn_switch_alarm);
}

// Check si l'alarme est à ON
void Buttons::initSwitchAlarm() {
	this->btn_switch_alarm.setEtat(this->btn_switch_alarm.readPinEtat());

	if (this->btn_switch_alarm.isPressed() == true) {
		this->p_alarm_g.isActivate = true;
		this->p_afficheur_g.displayAlarmeON();
		delay(1000);
	} else {
		this->p_alarm_g.isActivate = false;
		this->p_afficheur_g.displayAlarmeOFF();
		delay(1000);
	}
}

// Check de l'état d'un seul boutton
void Buttons::checkButtonAction(Button& p_btn) {
	bool tmp_etat = p_btn.readPinEtat();
	bool has_change = false;

	// Si l'état du bouton a changé
	if (tmp_etat != p_btn.etat) {
		p_btn.setEtat(tmp_etat);
		has_change = true;
	}

	switch (p_btn.pin) {
		// Boutton SET ALARM
		case PIN_BTN_ALARM_CONF :
			if (p_btn.isPressed() == true) {
				this->p_afficheur_g.displayAlarme();
			} else {
				this->p_afficheur_g.displayDate();
			}
		break;

		// Boutton ADD MM
		case PIN_BTN_ADD_MM :
			if (p_btn.isPressed() == true) {
				if (this->btn_alarm_conf.isPressed() == true) {
					this->p_afficheur_g.alarmAddMinute();
					delay(DELAY_PRESS_BUTTON);
				} else if (this->btn_time_conf.isPressed() == true) {
					this->p_afficheur_g.dateAddMinute();
					delay(DELAY_PRESS_BUTTON);
				}
			}
		break;

		// Boutton ADD HH
		case PIN_BTN_ADD_HH :
			if (p_btn.isPressed() == true) {
				if (this->btn_alarm_conf.isPressed() == true) {
					this->p_afficheur_g.alarmAddHeure();
					delay(DELAY_PRESS_BUTTON);
				} else if (this->btn_time_conf.isPressed() == true) {
					this->p_afficheur_g.dateAddHeure();
					delay(DELAY_PRESS_BUTTON);
				}
			}
		break;

		// Boutton SET TIME
		case PIN_BTN_TIME_CONF :
			if (p_btn.isPressed() == true) {
				if (this->p_afficheur_g.time_dot == true) {
					this->p_afficheur_g.time_dot = false;
				} else {
					this->p_afficheur_g.time_dot = true;
				}
			} else {
				this->p_afficheur_g.time_dot = true;
			}
		break;

		// Bouton pour la lumiére
		case PIN_BTN_LIGHT : 
			if (p_btn.isPressed() == true) {
				// Si l'alare est en route, on la coupe
				if(this->p_alarm_g.isRunning == true){
					this->p_alarm_g.stopAlarm();
					this->p_afficheur_g.displayAlameSTOP();              
					delay(DELAY_SHOW_MSG_STOP);
				}
			}			
		break;
		
		// Switch de l'larme
		case PIN_SWITCH_ALARM :
			if (has_change == true) {
				if (p_btn.isPressed() == true) {
					this->p_alarm_g.isActivate = true;
					this->p_afficheur_g.displayAlarmeON();
					delay(DELAY_SHOW_MSG_ON);
				} else {
					this->p_alarm_g.isActivate = false;
					if(this->p_alarm_g.isRunning == true){
						this->p_alarm_g.stopAlarm();
					}
					this->p_afficheur_g.displayAlarmeOFF();              
					delay(DELAY_SHOW_MSG_OFF);
				}
			}
		break;
	}
}
