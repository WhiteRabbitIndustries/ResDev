
#define DEBUG false


#include <FastLED.h>

#include "lightSpot.h"

#define LED_PIN     5
#define COLOR_ORDER GBR
#define CHIPSET     APA102
#define NUM_LEDS    67
#define DATA_PIN A4
#define CLOCK_PIN A5

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

#define GAMESPOTCOUNT 7
lightSpot gameSpots[GAMESPOTCOUNT];
// City Game light system:

int state = 0;






void setup() {
  
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  delay(1000); // sanity delay
  FastLED.addLeds<CHIPSET, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );


  if (DEBUG)
  {
    Serial.begin(57600);
    //  while (!Serial) {
    //    ; // wait for serial port to connect. Needed for Leonardo only
    //  }
    Serial.println("serial on");
  }
  for (int t = 0; t < GAMESPOTCOUNT; t++)
  {
    gameSpots[t] = lightSpot();
    gameSpots[t].lightSpotInitiate(t);

    switch (t)
    {
      case 0:
        gameSpots[t].ledNumber = 4;
        gameSpots[t].type = 2;
        gameSpots[t].gPal = CRGBPalette16( CRGB::Black, 0x448800, 0xFFAA00); // purple reddish: trip 0x777700 0xFFFF00

        break;
      case 1:
        gameSpots[t].ledNumber = 12;
        gameSpots[t].type = 0;
        gameSpots[t].gPal = CRGBPalette16( CRGB::Black, CRGB::Red, 0xFF0088); // red and yellow: crime
        break;
      case 2:
        gameSpots[t].ledNumber = 23;
        gameSpots[t].type = 1;
        gameSpots[t].gPal = CRGBPalette16( CRGB::Black, 0x00FF88, 0x00FFFF ); // greenish blue: ww
        break;
      case 3:
        gameSpots[t].ledNumber = 36;
        gameSpots[t].type = 2;
        gameSpots[t].gPal = CRGBPalette16( CRGB::Black, 0x448800, 0xFFAA00); // purple reddish: trip 0x777700 0xFFFF00
        break;
      case 4:
        gameSpots[t].ledNumber = 26;
        gameSpots[t].type = 0;
        gameSpots[t].gPal = CRGBPalette16( CRGB::Black, CRGB::White, CRGB::White); // red and yellow: crime
        break;
      case 5:
        gameSpots[t].ledNumber = 55;
        gameSpots[t].type = 1;
        gameSpots[t].gPal = CRGBPalette16( CRGB::Black, 0x00FF88, 0x00FFFF ); // greenish blue: ww
        break;
      case 6:
        gameSpots[t].ledNumber = 64;
        gameSpots[t].type = 1;
        gameSpots[t].gPal = CRGBPalette16( CRGB::Black, 0x00FF88, 0x00FFFF ); // greenish blue: ww
        break;
    }

  }



  //setButtons();
  //setLights();



  
  digitalWrite(13, HIGH);
}

unsigned long state4Click = 0;

void loop()
{
  //checkButtons();

  // chekc network conns
  // Get clients coming from server
 

  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
  //   static uint8_t hue = 0;
  //   hue++;
  //   CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
  //   CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
  //   gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);

  //update lightSpot values

  switch (state)
  {
    case 0:
      for (int t = 0; t < GAMESPOTCOUNT; t++)
      {
        gameSpots[t].update();
        reflect(t, HIGH);
      }

      break;
    case 1:
      FireFlicker(); // run simulation frame, using palette colors
      break;


    case 2:
      // scenario im untegrund
      for (int t = 0; t < GAMESPOTCOUNT ; t++)
      {
        if (gameSpots[t].type != 0) reflect(t, LOW);
        else
        {
          gameSpots[t].update();
          reflect(t, HIGH);
        }
      }
      break;
    case 3:
      // scenario wunderkammerwien
      // all off except ww
      for (int t = 0; t < GAMESPOTCOUNT ; t++)
      {
        //gameSpots[t].update();

        if (gameSpots[t].type != 1) reflect(t, LOW);
        else
        {
          gameSpots[t].update();
          reflect(t, HIGH);
        }
      }
      break;
    case 4:
      //// scenario in den schotten der nacht
      // increasing steps
      int seconds = (int)((millis() - state4Click) / 2000);
      if (DEBUG) Serial.println(seconds);
      CRGB color;
      byte colorindex = 0;
      switch (seconds)
      {
        case 0:
          for (int t = 0; t < GAMESPOTCOUNT ; t++)
          {
            if (gameSpots[t].type != 2) reflect(t, LOW);
            else
            {
              gameSpots[t].update();
              reflect(t, HIGH);
            }
          }
          color = ColorFromPalette( gameSpots[0].gPal, 0 );
          for (int t = 0; t < gameSpots[0].ledNumber ; t++)
          {
            leds[t] = color;
          }
          break;

        case 1:
          for (int t = 0; t < GAMESPOTCOUNT ; t++)
          {
            if (t != 0) reflect(t, LOW);
            else
            {
              gameSpots[t].update();
              reflect(t, HIGH);
            }
          }
          break;
        case 2:
          for (int t = 0; t < GAMESPOTCOUNT ; t++)
          {
            reflect(t, LOW);
          }
          gameSpots[0].update();
           colorindex = scale8( gameSpots[0].brightness, 240);
          color = ColorFromPalette( gameSpots[0].gPal, colorindex );
          leds[36] = color;
          break;

        case 3:
          //
          color = ColorFromPalette( gameSpots[0].gPal, 5 );
          leds[36] = color;
          
          gameSpots[0].update();
           colorindex = scale8( gameSpots[0].brightness, 240);
          color = ColorFromPalette( gameSpots[0].gPal, colorindex );
          leds[35] = color;
          break;
        case 4:
          color = ColorFromPalette( gameSpots[0].gPal, 5 );
          leds[35] = color;
          gameSpots[0].update();
           colorindex = scale8( gameSpots[0].brightness, 240);
          color = ColorFromPalette( gameSpots[0].gPal, colorindex );
          leds[34] = color;
          break;
        case 5:
          color = ColorFromPalette( gameSpots[0].gPal, 5 );
          leds[34] = color;
          gameSpots[0].update();
           colorindex = scale8( gameSpots[0].brightness, 240);
          color = ColorFromPalette( gameSpots[0].gPal, colorindex );
          leds[33] = color;
          break;
        case 6:
          color = ColorFromPalette( gameSpots[0].gPal, 5 );
          leds[33] = color;
          gameSpots[0].update();
           colorindex = scale8( gameSpots[0].brightness, 240);
          color = ColorFromPalette( gameSpots[0].gPal, colorindex );
          leds[32] = color;
          break;
        case 7:
          color = ColorFromPalette( gameSpots[0].gPal, 5 );
          leds[32] = color;
          gameSpots[0].update();
           colorindex = scale8( gameSpots[0].brightness, 240);
          color = ColorFromPalette( gameSpots[0].gPal, colorindex );
          leds[31] = color;
          break;
        case 8:
        
          color = ColorFromPalette( gameSpots[0].gPal, 5 );
          leds[31] = color;
          gameSpots[0].update();
           colorindex = scale8( gameSpots[0].brightness, 240);
          color = ColorFromPalette( gameSpots[0].gPal, colorindex );
          leds[30] = color;
          break;
          
         case 9:
         color = ColorFromPalette( gameSpots[0].gPal, 0 );
          leds[30] = color;
          leds[31] = color;
          leds[32] = color;
          leds[33] = color;
          leds[34] = color;
          leds[35] = color;
          leds[36] = color;
          
          
        state4Click = millis();  

          break;
          
      }


      // call steps function
      break;
  }






  int pstate = state;
  int secs = (millis()+80000)/20000;
  state = secs%5;
  if(pstate!=state)
  {
  
          
//            for (int k = 0; k++; k<NUM_LEDS)
//        {
//          CRGB color = CRGB::Black;
//          leds[k] = color;  
//        }
    state4Click = millis(); 
    Serial.print("state change at ");
    Serial.print(millis());
    Serial.print(" to state: ");
     Serial.println(state);
    
  }
    FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}




