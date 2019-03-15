/*!
* -----------------------------------------------------------
*                           CONFIGURATION
* -----------------------------------------------------------
* @file Conf.h
*
* Written by Sploutch
*
* BSD license, all text here must be included in any redistribution.
*
*/

#ifndef CONF
#define CONF

/* ----------------------------------------------------------- */
/*                          DELAIS                             */
/* ----------------------------------------------------------- */
// Delai de la boucle principale
const int DELAY_MAIN_LOOP = 100;

// Delai de pression sur un bouton
const int DELAY_PRESS_BUTTON = 150;

// delai d'affichage des ON, OFF, STOP
const int DELAY_SHOW_MSG_ON = 1000;
const int DELAY_SHOW_MSG_OFF = 1000;
const int DELAY_SHOW_MSG_STOP = 1000;


/* ----------------------------------------------------------- */
/*                          AFFICHEUR                          */
/* ----------------------------------------------------------- */
// Intensité par défault de l'affichage
const int DEFAULT_BRIGHTNESS = 0x0;


/* ----------------------------------------------------------- */
/*                          ALARME                             */
/* ----------------------------------------------------------- */
const int ALARM_NB_SEC_TO_INCREMENT = 1; // 1s = 4min15 pour arriver à 255
const int ALARM_NB_SEC_TO_STOP = 60*15;


/* ----------------------------------------------------------- */
/*                          PIN                                */
/* ----------------------------------------------------------- */
// CLOCK
const int PIN_DISPLAY_CLK = 9;
const int PIN_DISPLAY_DIO = 8;

// BOUTTON
const int PIN_BTN_ALARM_CONF  = 2;
const int PIN_BTN_ADD_HH      = 3;
const int PIN_BTN_ADD_MM      = 4;
const int PIN_BTN_TIME_CONF   = 5;
const int PIN_BTN_LIGHT       = 6;
const int PIN_SWITCH_ALARM    = 7;

// LED
const int PIN_LED_SUN = 10;

#endif