/*!
* -----------------------------------------------------------
*                           BUTTONS
* -----------------------------------------------------------
* @file Buttons.h
*
* Written by Sploutch
*
* BSD license, all text here must be included in any redistribution.
*
*/


#ifndef BUTTONS
#define BUTTONS

#include <Arduino.h>
#include "conf.h"
#include "Afficheur.h"
#include "Alarm.h"
#include "Button.h"

class Buttons {
public:
	// Pointeurs
	Afficheur &p_afficheur_g;
	Alarm &p_alarm_g;

	// Bouttons
	Button btn_alarm_conf;
	Button btn_add_mm;
	Button btn_add_hh;
	Button btn_time_conf;
	Button btn_light;
	Button btn_switch_alarm;

	// Init des bouttons
	Buttons(Afficheur& p_afficheur_g, Alarm& p_alarm_g);

	// Check de l'état de tous les bouttons
	void checkAllButtonsAction();

	// Check si l'alarme est à ON
	void initSwitchAlarm();

	// Check de l'état d'un seul boutton
	void checkButtonAction(Button& p_btn);
};

#endif