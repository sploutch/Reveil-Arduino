#include <TM1637Display.h>  // Bibliothèque pour l'écran 7 segements
#include <Wire.h>         // Bibliothèque pour l'I2C
#include <RTClib.h>     // Bibliothèque pour le module RTC de date

extern const int pin_CLK;
extern const int pin_DIO;

class Afficheur {
  public:

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
    int eeprom_alarm_hh;
    int eeprom_alarm_mm;
    int alarm_hh;
    int alarm_mm;
    int alarm_ss;
    bool alarm_dot;

    uint8_t seg_alarm_ON[4];

    uint8_t seg_alarm_OFF[4];
    
    uint8_t seg_alarm_STOP[4];
    
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
