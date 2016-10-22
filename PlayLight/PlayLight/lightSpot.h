#ifndef LIGHTSPOT_H
#define LIGHTSPOT_H

#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h"
#include <FastLED.h>

enum mode
{
  off, // pixel is oo
  active, // pixel is available for reach.
  passive
};



class lightSpot
{
  private:

    int brightness = 255;
    
    int pixelNumber; // value in mapped hexagonal top to center layout.
    int ledNumber; // raw led number on strip, etc...
    CRGB color = CRGB::Black;
    CRGBPalette16 gPal; // palette

    CRGB *ledStrip;
    mode pixelMode;



    // effect variables
    // COOLING: How much does the air cool as it rises? Less cooling = taller flames.  More cooling = shorter flames.
    // Default 55, suggested range 20-100
    int cooling = random8(50, 60); //55:results after pot tests for best aesthetic results

    // SPARKING: What chance (out of 255) is there that a new spark will be lit? Higher chance = more roaring fire.  Lower chance = more flickery fire.
    // Default 120, suggested range 50-200.
    int sparking = random8(25, 35);//30:results after pot tests for best aesthetic results
    
    
    
    int fadeBrightness = 0;
    int maxFadeBrightness = 200;
    int minFadeBrightness = 5;
    int fadeAmount = 1;
    long fadeDelay = 30; 
    long previousMillis = 0; 
    
  public:
    lightSpot();
    ~lightSpot();
    void initiate(int _pixelNumber, int _ledNumber, CRGB _ledStrip[], int stripLength);

    // Info:
    bool isOff();
    bool isOn();

    // Action
    void activate();
    void reach();
    void deactivate();

    void setBrightness(int _brightness);

    //Service
    void update();


    // Light Effects
    void flicker();
    void sleepBreath();
    void fadeToBlack();




};

#endif
