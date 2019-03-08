#ifndef BUTTON //https://fr.wikibooks.org/wiki/Programmation_C%2B%2B/Le_pr%C3%A9processeur
  #define BUTTON
  #include <Arduino.h>
  
  class Button {
    public:
      int pin;
      int etat;
  
      // Configuration de l'entrée du bouton
      void setPin(int pin);
  
      // Mise à jour de l'état du bouton
      void setEtat();
      
      // Mise à jour de l'état du bouton
      void setEtat(int etat);
  
      // Lecture de l'état du bouton
      bool readPinEtat();
  
      // Vérifie si le bouton est pressé ou non
      bool isPressed();
  };
#endif
