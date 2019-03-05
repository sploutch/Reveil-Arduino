#include <Arduino.h>

// Librairies
#include <Wire.h>         // Bibliothèque pour l'I2C
#include <RTClib.h>     // Bibliothèque pour le module RTC de date

// Fichier externes
#include "header.h"
#include "conf.cpp"

#include "Tools.h"

//#include "Afficheur.h"
#include "Afficheur.cpp"
#include "Alarm.cpp"


// Création de l'objet Afficheur
Afficheur g_afficheur(C_PIN_DISPLAY_CLK, C_PIN_DISPLAY_DIO);








// Initialisation de l'alarme
Alarm g_alarm;


/* ----------------------------------------------------------- */
/*                         BUTTON                              */
/* ----------------------------------------------------------- */
class Button {
  public:
    int pin;
    int etat;

    // Configuration de l'entrée du bouton
    void setPin(int pin) {
      this->pin = pin;
      pinMode(this->pin, INPUT);
    }

    // Mise à jour de l'état du bouton
    void setEtat() {
      this->etat = digitalRead(this->pin);
    }
    
    // Mise à jour de l'état du bouton
    void setEtat(int etat) {
      this->etat = etat;
    }

    // Lecture de l'état du bouton
    bool readPinEtat() {
      return digitalRead(this->pin);
    }

    // Vérifie si le bouton est pressé ou non
    bool isPressed() {
      if (this->etat == HIGH) {
        return false;
      } else {
        return true;
      }
    }
};


/* ----------------------------------------------------------- */
/*                        BUTTONS                              */
/* ----------------------------------------------------------- */
class Buttons {
  public:
    Button btn_alarm_conf;
    Button btn_add_mm;
    Button btn_add_hh;
    Button btn_time_conf;
    Button btn_switch_alarm;

    // Init des bouttons
    Buttons() {
      this->btn_alarm_conf.setPin(C_PIN_BTN_ALARM_CONF);
      this->btn_add_mm.setPin(C_PIN_BTN_ADD_MM);
      this->btn_add_hh.setPin(C_PIN_BTN_ADD_HH);
      this->btn_time_conf.setPin(C_PIN_BTN_TIME_CONF);
      this->btn_switch_alarm.setPin(C_PIN_SWITCH_ALARM);

      this->btn_alarm_conf.setEtat();
      this->btn_add_mm.setEtat();
      this->btn_add_hh.setEtat();
      this->btn_time_conf.setEtat();
      this->btn_switch_alarm.setEtat();
    }

    // Check de l'état de tous les bouttons
    void checkAllButtonsAction() {
      this->checkButtonAction(this->btn_alarm_conf);
      this->checkButtonAction(this->btn_add_mm);
      this->checkButtonAction(this->btn_add_hh);
      this->checkButtonAction(this->btn_time_conf);
      this->checkButtonAction(this->btn_switch_alarm);
    }

    // Check si l'alarme est à ON
    void initSwitchAlarm() {
      this->btn_switch_alarm.setEtat(this->btn_switch_alarm.readPinEtat());

      if (this->btn_switch_alarm.isPressed() == true) {
        g_alarm.isActivate = true;
        g_afficheur.displayAlarmeON();
        delay(1000);
      } else {
        g_alarm.isActivate = false;
        g_afficheur.displayAlarmeOFF();
        delay(1000);
      }
    }

    // Check de l'état d'un seul boutton
    void checkButtonAction(Button& btn) {
      bool tmp_etat = btn.readPinEtat();
      bool has_change = false;

      // Si l'état du bouton a changé
      if (tmp_etat != btn.etat) {
        btn.setEtat(tmp_etat);
        has_change = true;
      }

      switch (btn.pin) {
        // Boutton SET ALARM
        case C_PIN_BTN_ALARM_CONF :
          if (btn.isPressed() == true) {
            g_afficheur.displayAlarme();
          } else {
            g_afficheur.displayDate();
          }
          break;

        // Boutton ADD MM
        case C_PIN_BTN_ADD_MM :
          if (btn.isPressed() == true) {
            if (this->btn_alarm_conf.isPressed() == true) {
              g_afficheur.alarmAddMinute();
              delay(conf_delay_press_btn);
            } else if (this->btn_time_conf.isPressed() == true) {
              g_afficheur.dateAddMinute();
              delay(conf_delay_press_btn);
            }
          }
          break;

        // Boutton ADD HH
        case C_PIN_BTN_ADD_HH :
          if (btn.isPressed() == true) {
            if (this->btn_alarm_conf.isPressed() == true) {
              g_afficheur.alarmAddHeure();
              delay(conf_delay_press_btn);
            } else if (this->btn_time_conf.isPressed() == true) {
              g_afficheur.dateAddHeure();
              delay(conf_delay_press_btn);
            }
          }
          break;

        // Boutton SET TIME
        case C_PIN_BTN_TIME_CONF :
          if (btn.isPressed() == true) {
            if (g_afficheur.time_dot == true) {
              g_afficheur.time_dot = false;
            } else {
              g_afficheur.time_dot = true;
            }
          } else {
            g_afficheur.time_dot = true;
          }

          // Rien
          break;

        // Switch de l'larme
        case C_PIN_SWITCH_ALARM :
          if (has_change == true) {
            if (btn.isPressed() == true) {
              g_alarm.isActivate = true;
              g_afficheur.displayAlarmeON();
              delay(conf_delay_show_ON);
            } else {
              g_alarm.isActivate = false;
              if(g_alarm.isRunning == true){
                g_alarm.stopAlarm();
              }
              g_afficheur.displayAlarmeOFF();              
              delay(conf_delay_show_OFF);
            }
          }
          break;
      }
    }
};


// Initilisation des bouttons
Buttons g_bouttons;


/* ----------------------------------------------------------- */
/*                        SETUP/LOOP                           */
/* ----------------------------------------------------------- */
void setup() {
  Serial.begin(9600);
  pinMode(C_PIN_LED_SUN, OUTPUT);
  g_afficheur.refreshDate();
  g_bouttons.initSwitchAlarm();
  g_afficheur.displayCurrentTime();
}

void loop() {
  g_afficheur.refreshDate();
  g_bouttons.checkAllButtonsAction();
  g_alarm.autoCheck();
  delay(conf_loop_delay);
}
