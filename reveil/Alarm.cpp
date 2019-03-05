extern const int conf_nb_sec_to_stop;
extern const int conf_nb_sec_to_increment;
extern const int C_PIN_LED_SUN;
extern const int conf_delay_show_STOP;

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
