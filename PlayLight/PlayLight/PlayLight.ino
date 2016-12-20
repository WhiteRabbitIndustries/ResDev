/*
 * PLAYLIGHT CLASS
 * Functions and classes for controlling hex matrix.
 */

#define FASTLED_FORCE_SOFTWARE_SPI 1

#define DEBUG true


// LIGHT DECLARATIONS
//#define HEXSTRIP  72
//#define HEXMODE STRIP144

#include <FastLED.h>
#include "lightSpot.h"

//Strip properties:
#define COLOR_ORDER BGR
#define CHIPSET     APA102

// Number of pixels
//#if (defined(HEXSTRIP) && (HEXSTRIP == 72))
#define NUM_LEDS    61
//#elif (defined( (HEXSTRIP) && (HEXSTRIP == 144))
// #define NUM_LEDS    113
//#endif

// Using hardware SPI on Teensy, so varilables are not defined
//#define DATA_PIN 11
//#define CLOCK_PIN 13

#define BRIGHTNESS  127
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];





// COMM DECLARATIONS
#include <Wire.h>
#define I2C_ADDRESS_PLAYLIGHT 0x9

// GAMEPLAY
enum state
{
  animation,
  game
};


state playLightState = animation;


// Declaration

/*
lightSpot* locSpots = 0;
int locSpotsCount = 0;
*/
lightSpot locSpots[NUM_LEDS];
int locSpotsCount = NUM_LEDS;




const int spiralMap[NUM_LEDS] =
{
  34, 35, 49, 50, 60, 59, 58, 57, 56, 55, 43, 42, 26, 25, 11, 10, 0,  1,  2,  3,  4,  5,  17, 18,
  33, 36, 48, 51, 52, 53, 54, 44, 41, 27, 24, 12, 9,  8,  7,  6,  16, 19,
  32, 37, 47, 46, 45, 40, 28, 23, 13, 14, 15, 20,
  31, 38, 39, 29, 22, 21,
  30
};





void setup()
{
  delay(500); // sanity delay

  // Passive locSpot Declaration: (comment out for dynamic declaration)
  for (int i = 0; i < NUM_LEDS; i++)
  {
    locSpots[i].initiate(i, spiralMap[i], leds, NUM_LEDS);
  }




  // LIGHT INIT
  //FastLED.addLeds<CHIPSET, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );


  // COMM INIT
  Wire.begin(I2C_ADDRESS_PLAYLIGHT);                // join i2c bus with address #9
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event


  if (DEBUG)
  {
    Serial.begin(57600);
    Serial.println("serial on");
  }



}

void loop()
{

  // Add entropy to random number generator; we use a lot of it.



  switch (playLightState)
  {
    case animation:
      //hexAnimate();
      break;

    case game:

      for (int t = 0; t < locSpotsCount ; t++)
      {
        locSpots[t].update();
      }
      break;

    default:

      break;

  }






  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void hexAnimate()
{
  //locSpots
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    if (i >= 0 && i < 24)
    {}
    else if (i >= 24 && i < 42)
    {}
    else if (i >= 42 && i < 60)
    {}
    else if (i==60)
    {}
    
  }

}


