#include <Arduino.h>

#include <TM1637Display.h>  // Bibliothèque pour l'écran 7 segements
#include "Wire.h"         // Bibliothèque pour l'I2C
#include "RTClib.h"     // Bibliothèque pour le module RTC de date
#include "EEPROM.h"       // Bibliothèque pour la mémoire interne de l'Arduino

// Fichier externes
#include "header.h"
#include "conf.c"


/* ----------------------------------------------------------- */
/*                          EEPROM                             */
/* ----------------------------------------------------------- */
// Effacement de l'EEEPROM
void eeprom_clear() {
  for (int i = 0; i < 256; i++) {
    EEPROM.write(i, 0);
  }
}

// Recupération d'une valeur stockée
int eeprom_get(int add) {
  return (int) EEPROM.read(add);
}

// Stockage du valeur
void eeprom_set(int add, int val) {
  EEPROM.write(add, val);
}


/* ----------------------------------------------------------- */
/*                        AFFICHEUR                            */
/* ----------------------------------------------------------- */
class Afficheur {
  public:
    // Mode
    bool dateIsDisplayed;
    bool alarmIsDisplayed;

    // Module
    TM1637Display display;
    RTC_DS1307 rtc;

    // Conf
    int defaut_brightness;

    // Date
    DateTime current_dateTime;
    bool time_dot;
    int current_hh;
    int current_mm;
    int current_ss;

    // Alarme
    int eeprom_alarm_hh = 0;
    int eeprom_alarm_mm = 1;
    int alarm_hh;
    int alarm_mm;
    int alarm_ss;
    bool alarm_dot;

    uint8_t seg_alarm_ON[4] = {
      0,
      0,
      SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
      SEG_C | SEG_E | SEG_G
    };

    uint8_t seg_alarm_OFF[4] = {
      0,
      SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
      SEG_A | SEG_E | SEG_F | SEG_G,
      SEG_A | SEG_E | SEG_F | SEG_G
    };
    
    uint8_t seg_alarm_STOP[4] = {
      SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
      SEG_F | SEG_G | SEG_E | SEG_D,
      SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
      SEG_A | SEG_B | SEG_F | SEG_G | SEG_E,
    };

    // Constructeur
    Afficheur(int pin_CLK, int pin_DIO) : display(pin_CLK, pin_DIO) {
      // Alarme init
      this->alarm_hh = eeprom_get(this->eeprom_alarm_hh);
      this->alarm_mm = eeprom_get(this->eeprom_alarm_mm);
      this->alarm_ss = 0;
      this->alarm_dot = true;

      // Time init
      this->current_hh = 23;
      this->current_mm = 59;
      this->current_ss = 00;
      this->time_dot = true;
      this->dateIsDisplayed = true;
      this->alarmIsDisplayed = false;
      this->defaut_brightness = 0x00;
      this->display.setBrightness(this->defaut_brightness);
      this->clear();
      Wire.begin();
      this->rtc.begin();
    }

    // Effacement de l'Afficheur
    void clear() {
      uint8_t tab_ClearDisplay[] = {0x0, 0x0, 0x0, 0x0};
      this->display.setSegments(tab_ClearDisplay);
    }

    // Afficher hh:mm
    void afficher_time(int heure, int minute) {
      /*
        Aucun 0b00000000
        Double millieu 0b11100000
      */
      if (this->time_dot == true) {
        this->display.showNumberDecEx(heure * 100 + minute, 0b11100000, true);
      } else {
        this->display.showNumberDecEx(heure * 100 + minute, 0b00000000, true);
      }
    }

    // Mise à jour de la date
    void refreshDate() {
      this->current_dateTime = this->rtc.now();
      this->current_ss = this->current_dateTime.second();

      int tmp_mm = this->current_dateTime.minute();
      int tmp_hh = this->current_dateTime.hour();

      // L'heure a changé
      if (tmp_hh != this->current_hh || tmp_mm != this->current_mm) {
        this->current_hh = tmp_hh;
        this->current_mm = tmp_mm;

        if (dateIsDisplayed == true) {
          this->displayCurrentTime();
        }
      }
    }

    // Affichage de la date
    void displayDate() {
      this->dateIsDisplayed = true;
      this->alarmIsDisplayed = false;
      this->displayCurrentTime();
    }

    // Affichage de l'alarme
    void displayAlarme() {
      this->dateIsDisplayed = false;
      this->alarmIsDisplayed = true;
      if (this->alarm_dot == true) {
        this->display.showNumberDecEx(alarm_hh * 100 + this->alarm_mm, 0b11100000, true);
      } else {
        this->display.showNumberDecEx(alarm_hh * 100 + this->alarm_mm, 0b00000000, true);
      }

      if (this->alarm_dot == true) {
        this->alarm_dot = false;
      } else {
        this->alarm_dot = true;
      }
    }

    // Affichage du texte : ON
    void displayAlarmeON() {
      this->display.setSegments(this->seg_alarm_ON);
    }

    // Affichage du texte : OFF
    void displayAlarmeOFF() {
      this->display.setSegments(this->seg_alarm_OFF);
    }

    // Affichage du texte : STOP
    void displayAlameSTOP(){
      this->display.setSegments(this->seg_alarm_STOP);
    }

    // Ajout d'une minute à l'alarme
    void alarmAddMinute() {
      this->alarm_mm = this->alarm_mm + 1;
      if (this->alarm_mm >= 60) {
        this->alarm_mm = this->alarm_mm - 60;
        this->alarm_hh = this->alarm_hh + 1;
      }
      if (this->alarm_hh >= 24) {
        this->alarm_hh = this->alarm_hh - 24;
      }
      eeprom_set(this->eeprom_alarm_hh, this->alarm_hh);
      eeprom_set(this->eeprom_alarm_mm, this->alarm_mm);
      this->displayAlarme();
    }

    // Ajout d'une heure à l'alarme
    void alarmAddHeure() {
      this->alarm_hh = this->alarm_hh + 1;
      if (this->alarm_hh >= 24) {
        this->alarm_hh = this->alarm_hh - 24;
      }
      eeprom_set(this->eeprom_alarm_hh, this->alarm_hh);
      this->displayAlarme();
    }

    // Affichage de l'heure courante
    void displayCurrentTime() {
      afficher_time(this->current_hh, this->current_mm);
    }

    // Ajustement auto de l'heure en fonction de la date de complation du fichier
    void autoAdjustDate() {
      this->rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      this->refreshDate();
    }

    // Ajsutement manuel de l'heure
    void manualAdjustDate(int YYYY, int MM, int DD, int hh, int mm, int ss) {
      this->rtc.adjust(DateTime(YYYY, MM, DD, hh, mm, ss));
      this->refreshDate();
    }

    // Ajout d'une minute à la date
    void dateAddMinute() {
      this->current_dateTime = this->rtc.now();

      int tmp_YYYY = this->current_dateTime.year();
      int tmp_MM   = this->current_dateTime.month();
      int tmp_DD   = this->current_dateTime.day();
      int tmp_hh   = this->current_dateTime.hour();
      int tmp_mm   = this->current_dateTime.minute();
      int tmp_ss   = 0;

      tmp_mm = tmp_mm + 1;
      if (tmp_mm >= 60) {
        tmp_mm = tmp_mm - 60;
      }

      this->rtc.adjust(DateTime(tmp_YYYY, tmp_MM, tmp_DD, tmp_hh, tmp_mm, tmp_ss));
      this->refreshDate();
    }

    // Ajout d'une heure à la date
    void dateAddHeure() {
      this->current_dateTime = this->rtc.now();

      int tmp_YYYY = this->current_dateTime.year();
      int tmp_MM   = this->current_dateTime.month();
      int tmp_DD   = this->current_dateTime.day();
      int tmp_hh   = this->current_dateTime.hour();
      int tmp_mm   = this->current_dateTime.minute();
      int tmp_ss   = this->current_dateTime.second();

      tmp_hh = tmp_hh + 1;
      if (tmp_hh >= 24) {
        tmp_hh = tmp_hh - 24;
      }

      this->rtc.adjust(DateTime(tmp_YYYY, tmp_MM, tmp_DD, tmp_hh, tmp_mm, tmp_ss));
      this->refreshDate();
    }
};


// Création de l'objet Afficheur
Afficheur g_afficheur(C_PIN_DISPLAY_CLK, C_PIN_DISPLAY_DIO);


/* ----------------------------------------------------------- */
/*                        ALARME                               */
/* ----------------------------------------------------------- */
class Alarm {
  public:
    bool isActivate = false;
    bool isRunning = false;

    int hh_running = 0;
    int mm_running = 0;
    int ss_running = 0;
    int sum_ss_running = 0;

    int intensity = 0;

    // Auto vérification de l'arame : ON, OFF, Intensité
    void autoCheck() {
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
    void calcul_duree_running() {
      if (this->isRunning == false) {
        return;
      }
      
      // Avant minuit
      if (g_afficheur.alarm_hh <= g_afficheur.current_hh) {
        this->hh_running = g_afficheur.current_hh - g_afficheur.alarm_hh;
      } else {
        this->hh_running = (g_afficheur.current_hh + 24) - g_afficheur.alarm_hh;
      }

      // L'heure est pas passée
      if (g_afficheur.alarm_mm <= g_afficheur.current_mm) {
        this->mm_running = g_afficheur.current_mm - g_afficheur.alarm_mm;
      } else {
        this->mm_running = (g_afficheur.current_mm + 60) - g_afficheur.alarm_mm;
        this->hh_running--;
      }

      // La minute est passée
      if (g_afficheur.alarm_ss <= g_afficheur.current_ss) {
        this->ss_running = g_afficheur.current_ss - g_afficheur.alarm_ss;
      } else {
        this->ss_running = (g_afficheur.current_ss + 60) - g_afficheur.alarm_ss;
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
    bool checkIfAlarmNeedStart() {
      if (this->isActivate == false) {
        return false;
      }
      if (g_afficheur.alarm_hh == g_afficheur.current_hh && g_afficheur.alarm_mm == g_afficheur.current_mm && g_afficheur.alarm_ss == g_afficheur.current_ss) {
        return true;
      }
      return false;
    }

    // Controle si l'alarme doit s'arréter
    void checkIfAlarmNeedStop() {
      if (this->isRunning == true) {
        if (this->sum_ss_running >= conf_nb_sec_to_stop) {
          Serial.print("Alarme Need Stop\n");
          this->stopAlarm();
        }
      }
    }

    // Arrete de l'alarme
    void stopAlarm() {
      Serial.print("Alarme Stop\n");
      analogWrite(C_PIN_LED_SUN, 0);

      this->isRunning = false;
      this->hh_running = 0;
      this->mm_running = 0;
      this->ss_running = 0;
      this->sum_ss_running = 0;

      g_afficheur.displayAlameSTOP();
      delay(conf_delay_show_STOP);
    }

    // Démarage de l'alarme
    void startAlarm() {
      Serial.print("Alarme Start\n");
      this->isRunning = true;
      this->intensity = 1;
      this->augmenterIntensite();
    }

    // Augmentation de l'intensité de la lumiére
    void augmenterIntensite() {
      if (this->intensity < 255 && this->isRunning == true) {
        float new_val = (float)255.00 * ((float)(this->sum_ss_running + 1) / (float)(conf_nb_sec_to_increment * 255));
        if (new_val > 255.00) {
          new_val = 255.00;
        }
        if (new_val <= 0.00) {
          new_val = 1.00;
        }

        if ((int)new_val != this->intensity) {
          this->intensity = (int)new_val;
          analogWrite(C_PIN_LED_SUN, this->intensity);
          Serial.print("Intensity set to : ");
          Serial.print(this->intensity);
          Serial.print("\n");
        }
      }
    }
};


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
