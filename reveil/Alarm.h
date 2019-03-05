extern Afficheur g_afficheur;

extern const int conf_nb_sec_to_stop;
extern const int conf_nb_sec_to_increment;
extern const int C_PIN_LED_SUN;
extern const int conf_delay_show_STOP;

class Alarm {
  public:
    bool isActivate;
    bool isRunning;

    int hh_running;
    int mm_running;
    int ss_running;
    int sum_ss_running;

    int intensity;

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
