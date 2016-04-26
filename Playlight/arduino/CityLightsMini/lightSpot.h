#ifndef LIGHTSPOT_H
#define LIGHTSPOT_H
 
#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h"
 #include <FastLED.h>
 
class lightSpot {
  private:


  
  

public:

  int brightness;
  int id;
  int type; // 0: im ug, 1: ww, 2: nacht
  CRGBPalette16 gPal; // palette
  int ledNumber; 
int fadeAmount = 2;
  CRGB gameType;
  
        lightSpot();
        ~lightSpot();
        void lightSpotInitiate(int _id);
        void setColor();
        void update();
//        void on();
//        void off();
//        void blink(int time);



};
 
#endif
