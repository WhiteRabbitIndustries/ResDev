#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


#define PIN 2
#define NUM_LEDS 113
#define HEX_LEDS 61
#define LEDBRIGHTNESS 31
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

int brightness = 0;    // how bright the LED is
int fadeAmount = 1;    // how many points to fade the LED by

int gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
  
  int base = 56;
int shell1[] = {41, 39, 58, 71, 73, 54};
int shell2[] = {24, 26, 28, 37, 60, 69, 88, 86, 84, 75, 52, 43};
int shell3[] = {17, 15, 13, 11, 30, 35, 62, 67, 90, 95, 97, 99, 101,  82, 77, 50, 45, 22};
int shell4[] = {0,  2,  4,  6,  8,  9,  32, 33, 64, 65, 92, 93, 112,  110,  108,  106,  104,  103,  80, 79, 48, 47, 20, 19};


  
void setup() {
  // put your setup code here, to run once:
 strip.setBrightness(LEDBRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int brightnessB = 0;
int fadeAmountB = fadeAmount;
int brightnessS1 = 31;
int fadeAmountS1 = fadeAmount;
int brightnessS2 = 63;
int fadeAmountS2 = fadeAmount;
int brightnessS3 = 127;
int fadeAmountS3 = fadeAmount;
int brightnessS4 = 156;
int fadeAmountS4 = fadeAmount;

void loop() {
  // put your main code here, to run repeatedly:


  strip.setPixelColor(base, strip.Color(0,0,0,brightnessB));
  brightnessB = brightnessB + fadeAmountB;
      if (brightnessB == 0 || brightnessB == 255) {
        fadeAmountB = -fadeAmountB ;
      }
      
  
  for (int i = 0; i<(sizeof(shell1)/sizeof(int));i++)
    {
      strip.setPixelColor(shell1[i], strip.Color(0,0,0,brightnessS1));
      
    }
    brightnessS1 = brightnessS1 + fadeAmountS1;
      if (brightnessS1 == 0 || brightnessS1 == 255) {
        fadeAmountS1 = -fadeAmountS1 ;
      }

  for (int i = 0; i<(sizeof(shell2)/sizeof(int));i++)
    {
      strip.setPixelColor(shell2[i], strip.Color(brightnessS2,0,brightnessS2,brightnessS2));
      
    }
    brightnessS2 = brightnessS2 + fadeAmountS2;
      if (brightnessS2 == 0 || brightnessS2 == 255) {
        fadeAmountS2 = -fadeAmountS2 ;
      }
      
      for (int i = 0; i<(sizeof(shell3)/sizeof(int));i++)
    {
      strip.setPixelColor(shell3[i], strip.Color(brightnessS3,brightnessS3/2,0,brightnessS3));
      
    }
    brightnessS3 = brightnessS3 + fadeAmountS3;
      if (brightnessS3 == 0 || brightnessS3 == 255) {
        fadeAmountS3 = -fadeAmountS3 ;
      }
      
      for (int i = 0; i<(sizeof(shell4)/sizeof(int));i++)
    {
      strip.setPixelColor(shell4[i], strip.Color(0,brightnessS4,brightnessS4,brightnessS4));
      
    }
    brightnessS4 = brightnessS4 + fadeAmountS4;
      if (brightnessS4 == 0 || brightnessS4 == 255) {
        fadeAmountS4 = -fadeAmountS4 ;
      }
      


      
strip.show();


/*
    brightness = brightness + fadeAmount;
    
      // reverse the direction of the fading at the ends of the fade:
      if (brightness == 0 || brightness == 255) {
        fadeAmount = -fadeAmount ;
      }
      // wait for 30 milliseconds to see the dimming effect
      
    
    // change the brightness for next time through the loop:
    //for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
    int i = 58;
        strip.setPixelColor(i, strip.Color(brightness,0,brightness, 0 ));
        
        //delay(wait);
      }
      strip.show();
      
      delay(3);
      
      */
     /* 
      int del = 50;
      colorWipe(strip.Color(255, 0, 0,0), del);
      colorWipe(strip.Color(0, 0, 0,0), del);
      colorWipe(strip.Color(0, 0, 0,255), del);
      colorWipe(strip.Color(0, 0, 0,0), del);
      colorWipe(strip.Color(255, 0, 255,0), del);
      colorWipe(strip.Color(0, 0, 0,0), del);
      */
      
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<HEX_LEDS; i++) {
    strip.setPixelColor(mappedSpiral(i), c);
    strip.show();
    delay(wait);
  }
}

int modBrightness(int _brightness)
{
  
  _brightness = _brightness + fadeAmount;
    
      // reverse the direction of the fading at the ends of the fade:
      if (_brightness == 0 || _brightness == 255) {
        fadeAmount = -fadeAmount ;
      }
      return _brightness;
  }


int spiralMap[] = {
  0,  2,  4,  6,  8,  9,  32, 33, 64, 65, 92, 93, 112,  110,  108,  106,  104,  103,  80, 79, 48, 47, 20, 19,
  17, 15, 13, 11, 30, 35, 62, 67, 90, 95, 97, 99, 101,  82, 77, 50, 45, 22, 
  24, 26, 28, 37, 60, 69, 88, 86, 84, 75, 52, 43, 
  41, 39, 58, 71, 73, 54,
  56
  };

int mappedSpiral(int i)
{
  return spiralMap[i];
  }
