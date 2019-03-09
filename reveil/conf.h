#ifndef CONF
  #define CONF

  /* ----------------------------------------------------------- */
  /*                          VARiABLES                          */
  /* ----------------------------------------------------------- */
  // Delai de la boucle principale
  const int conf_loop_delay = 100;
  
  // Delai de pression sur un bouton
  const int conf_delay_press_btn = 150;
  
  // delai d'affichage des ON, OFF, STOP
  const int conf_delay_show_ON = 1000;
  const int conf_delay_show_OFF = 1000;
  const int conf_delay_show_STOP = 1000;
  
  // Alarme
  const int conf_nb_sec_to_increment = 1; // 1s = 4min15 pour arriver à 255
  const int conf_nb_sec_to_stop = 60*15;
  
  
  /* ----------------------------------------------------------- */
  /*                          PIN                                */
  /* ----------------------------------------------------------- */
  // CLOCK
  const int C_PIN_DISPLAY_CLK = 9;
  const int C_PIN_DISPLAY_DIO = 8;
  
  // BOUTTON
  const int C_PIN_BTN_ALARM_CONF  = 2;
  const int C_PIN_BTN_ADD_HH      = 3;
  const int C_PIN_BTN_ADD_MM      = 4;
  const int C_PIN_BTN_TIME_CONF   = 5;
  const int C_PIN_BTN_LIGHT       = 6;
  const int C_PIN_SWITCH_ALARM    = 7;
  
  // LED
  const int C_PIN_LED_SUN = 10;
#endif
