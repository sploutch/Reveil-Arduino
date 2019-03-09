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
      Afficheur &g_afficheur;
      Alarm &g_alarm;

      // Bouttons
      Button btn_alarm_conf;
      Button btn_add_mm;
      Button btn_add_hh;
      Button btn_time_conf;
      Button btn_light;
      Button btn_switch_alarm;
  
      // Init des bouttons
      Buttons(Afficheur& afficheur_ref, Alarm& alarm_ref);

      // Check de l'état de tous les bouttons
      void checkAllButtonsAction();
  
      // Check si l'alarme est à ON
      void initSwitchAlarm();
  
      // Check de l'état d'un seul boutton
      void checkButtonAction(Button& btn);
  };
#endif
