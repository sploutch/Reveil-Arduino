#ifndef ALARM
  #define ALARM

  #include "conf.h"
  #include "Afficheur.h"
    
  class Alarm {
    public:
      bool isActivate = false;
      bool isRunning = false;
      Afficheur& g_afficheur;
      int hh_running = 0;
      int mm_running = 0;
      int ss_running = 0;
      int sum_ss_running = 0;
  
      int intensity = 0;
      
      Alarm(Afficheur &g_afficheur);
      
      // Auto vérification de l'arame : ON, OFF, Intensité
      void autoCheck();
  
      // Calcul de la durée de sonnerie
      void calcul_duree_running();
  
      // Controle si l'alarme doit être demarrée
      bool checkIfAlarmNeedStart();
  
      // Controle si l'alarme doit s'arréter
      void checkIfAlarmNeedStop();
  
      // Arrete de l'alarme
      void stopAlarm();
  
      // Démarage de l'alarme
      void startAlarm();
  
      // Augmentation de l'intensité de la lumiére
      void augmenterIntensite();
  };
#endif // ALARM
