/*!
* -----------------------------------------------------------
*                           AFFICHEUR
* -----------------------------------------------------------
* @file Afficheur.h
*
* Written by Sploutch
*
* BSD license, all text here must be included in any redistribution.
*
*/

#ifndef AFFICHEUR
#define AFFICHEUR

#include <TM1637Display.h>  // Bibliothèque pour l'écran 7 segements
#include <Wire.h>         // Bibliothèque pour l'I2C
#include <RTClib.h>     // Bibliothèque pour le module RTC de date

#include "conf.h"
#include "Tools.h"


class Afficheur {
public:  
	// Module
	TM1637Display display;
	RTC_DS1307 rtc;

	// Conf
	int current_brightness = DEFAULT_BRIGHTNESS;
	
	// Date
	DateTime current_dateTime;
	bool dateIsDisplayed;
	bool time_dot;
	int current_hh;
	int current_mm;
	int current_ss;

	// Alarme
	bool alarmIsDisplayed;
	int eeprom_alarm_hh = 0;
	int eeprom_alarm_mm = 1;
	int alarm_hh;
	int alarm_mm;
	int alarm_ss;
	bool alarm_dot;

	
	uint8_t sealarm_g_ON[4]= {
		0,
		0,
		SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
		SEG_C | SEG_E | SEG_G
	};

	uint8_t sealarm_g_OFF[4]= {
		0,
		SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
		SEG_A | SEG_E | SEG_F | SEG_G,
		SEG_A | SEG_E | SEG_F | SEG_G
	};
	
	uint8_t sealarm_g_STOP[4]= {
		SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
		SEG_F | SEG_G | SEG_E | SEG_D,
		SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
		SEG_A | SEG_B | SEG_F | SEG_G | SEG_E,
	};
	
	// Constructeur
	Afficheur(int pin_CLK, int pin_DIO);

	// Effacement de l'Afficheur
	void clear();

	// Afficher hh:mm
	void afficher_time(int heure, int minute);

	// Mise à jour de la date
	void refreshDate();

	// Affichage de la date
	void displayDate();

	// Affichage de l'alarme
	void displayAlarme();

	// Affichage du texte : ON
	void displayAlarmeON();

	// Affichage du texte : OFF
	void displayAlarmeOFF();
	
	// Affichage du texte : STOP
	void displayAlameSTOP();

	// Ajout d'une minute à l'alarme
	void alarmAddMinute();

	// Ajout d'une heure à l'alarme
	void alarmAddHeure();
	// Affichage de l'heure courante
	void displayCurrentTime();

	// Ajustement auto de l'heure en fonction de la date de complation du fichier
	void autoAdjustDate();

	// Ajsutement manuel de l'heure
	void manualAdjustDate(int YYYY, int MM, int DD, int hh, int mm, int ss);

	// Ajout d'une minute à la date
	void dateAddMinute();

	// Ajout d'une heure à la date
	void dateAddHeure();
};

#endif