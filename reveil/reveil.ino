/*!
* -----------------------------------------------------------
*                           MAIN
* -----------------------------------------------------------
* @file reveil.ino
*
* Written by Sploutch
*
* BSD license, all text here must be included in any redistribution.
*
*/

#include <Arduino.h>

#include "conf.h"
#include "Tools.h"
#include "Afficheur.h"
#include "Alarm.h"
#include "Button.h"
#include "Buttons.h"

// Initialisation des Objets globaux
Afficheur afficheur_g(PIN_DISPLAY_CLK, PIN_DISPLAY_DIO);
Alarm alarm_g(afficheur_g);
Buttons buttons_g(afficheur_g, alarm_g);


/* ----------------------------------------------------------- */
/*                        SETUP/LOOP                           */
/* ----------------------------------------------------------- */
void setup() {
	Serial.begin(9600);
	pinMode(PIN_BTN_ALARM_CONF, INPUT);
	pinMode(PIN_BTN_ADD_HH,     INPUT);
	pinMode(PIN_BTN_ADD_MM,     INPUT);
	pinMode(PIN_BTN_TIME_CONF,  INPUT);
	pinMode(PIN_BTN_LIGHT,      INPUT);
	pinMode(PIN_SWITCH_ALARM,   INPUT);

	pinMode(PIN_LED_SUN,        OUTPUT);

	afficheur_g.refreshDate();
	buttons_g.initSwitchAlarm();
	afficheur_g.displayCurrentTime();
}

void loop() {
	afficheur_g.refreshDate();
	buttons_g.checkAllButtonsAction();
	alarm_g.autoCheck();
	delay(DELAY_MAIN_LOOP);
}