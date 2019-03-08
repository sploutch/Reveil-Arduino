#include <Arduino.h>


// Fichier externes
#include "conf.h"

#include "Tools.h"

#include "Afficheur.h"
#include "Alarm.h"
#include "Button.h"
#include "Buttons.h"

// Initialisation de l'afficheur
Afficheur g_afficheur(C_PIN_DISPLAY_CLK, C_PIN_DISPLAY_DIO);

// Initialisation de l'alarme
Alarm g_alarm(g_afficheur);

// Initilisation des bouttons
Buttons g_buttons(g_afficheur, g_alarm);


/* ----------------------------------------------------------- */
/*                        SETUP/LOOP                           */
/* ----------------------------------------------------------- */
void setup() {
  Serial.begin(9600);
  pinMode(C_PIN_LED_SUN, OUTPUT);
  g_afficheur.refreshDate();
  g_buttons.initSwitchAlarm();
  g_afficheur.displayCurrentTime();
}

void loop() {
  g_afficheur.refreshDate();
  g_buttons.checkAllButtonsAction();
  g_alarm.autoCheck();
  delay(conf_loop_delay);
}
