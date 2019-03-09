#include "Buttons.h"


/* ----------------------------------------------------------- */
/*                        BUTTONS                              */
/* ----------------------------------------------------------- */

// Init des bouttons
Buttons::Buttons(Afficheur& afficheur_ref, Alarm& alarm_ref) : g_afficheur(afficheur_ref), g_alarm(alarm_ref) {
  this->btn_alarm_conf.setPin(C_PIN_BTN_ALARM_CONF);
  this->btn_add_mm.setPin(C_PIN_BTN_ADD_MM);
  this->btn_add_hh.setPin(C_PIN_BTN_ADD_HH);
  this->btn_time_conf.setPin(C_PIN_BTN_TIME_CONF);
  this->btn_light.setPin(C_PIN_BTN_LIGHT);
  this->btn_switch_alarm.setPin(C_PIN_SWITCH_ALARM);

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
    this->g_alarm.isActivate = true;
    this->g_afficheur.displayAlarmeON();
    delay(1000);
  } else {
    this->g_alarm.isActivate = false;
    this->g_afficheur.displayAlarmeOFF();
    delay(1000);
  }
}

// Check de l'état d'un seul boutton
void Buttons::checkButtonAction(Button& btn) {
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
        this->g_afficheur.displayAlarme();
      } else {
        this->g_afficheur.displayDate();
      }
      break;

    // Boutton ADD MM
    case C_PIN_BTN_ADD_MM :
      if (btn.isPressed() == true) {
        if (this->btn_alarm_conf.isPressed() == true) {
          this->g_afficheur.alarmAddMinute();
          delay(conf_delay_press_btn);
        } else if (this->btn_time_conf.isPressed() == true) {
          this->g_afficheur.dateAddMinute();
          delay(conf_delay_press_btn);
        }
      }
      break;

    // Boutton ADD HH
    case C_PIN_BTN_ADD_HH :
      if (btn.isPressed() == true) {
        if (this->btn_alarm_conf.isPressed() == true) {
          this->g_afficheur.alarmAddHeure();
          delay(conf_delay_press_btn);
        } else if (this->btn_time_conf.isPressed() == true) {
          this->g_afficheur.dateAddHeure();
          delay(conf_delay_press_btn);
        }
      }
      break;

    // Boutton SET TIME
    case C_PIN_BTN_TIME_CONF :
      if (btn.isPressed() == true) {
        if (this->g_afficheur.time_dot == true) {
          this->g_afficheur.time_dot = false;
        } else {
          this->g_afficheur.time_dot = true;
        }
      } else {
        this->g_afficheur.time_dot = true;
      }
      break;

    // Bouton pour la lumiére
    case C_PIN_BTN_LIGHT : 
      if (btn.isPressed() == true) {
          // Si l'alare est en route, on la coupe
          if(this->g_alarm.isRunning == true){
            this->g_alarm.stopAlarm();
            this->g_afficheur.displayAlameSTOP();              
            delay(conf_delay_show_STOP);
          }
      }

    break;
    // Switch de l'larme
    case C_PIN_SWITCH_ALARM :
      if (has_change == true) {
        if (btn.isPressed() == true) {
          this->g_alarm.isActivate = true;
          this->g_afficheur.displayAlarmeON();
          delay(conf_delay_show_ON);
        } else {
          this->g_alarm.isActivate = false;
          if(this->g_alarm.isRunning == true){
            this->g_alarm.stopAlarm();
          }
          this->g_afficheur.displayAlarmeOFF();              
          delay(conf_delay_show_OFF);
        }
      }
      break;
  }
}
