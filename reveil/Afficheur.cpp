#include <TM1637Display.h>  // Bibliothèque pour l'écran 7 segements
#include <Wire.h>         // Bibliothèque pour l'I2C
#include <RTClib.h>     // Bibliothèque pour le module RTC de date

#include "Tools.h"

extern const int C_PIN_DISPLAY_CLK;
extern const int C_PIN_DISPLAY_DIO;

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
    Afficheur(int pin_CLK, int pin_DIO): display(pin_CLK, pin_DIO) {
      
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
