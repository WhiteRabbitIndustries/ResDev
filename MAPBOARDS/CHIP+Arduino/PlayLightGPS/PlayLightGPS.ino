// CITY GAMES
// Arduino Nano + Ultimate GPS + LEDHEX(SK6822)
// GPS and LED library from Adafruit


#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences.
#define GPSECHO  false

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = true;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


#define PIN 5
#define NUM_LEDS 113
#define HEX_LEDS 61
#define LEDBRIGHTNESS 7
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
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

int base = 56;
int shell1[] = {41, 39, 58, 71, 73, 54};
int shell2[] = {24, 26, 28, 37, 60, 69, 88, 86, 84, 75, 52, 43};
int shell3[] = {17, 15, 13, 11, 30, 35, 62, 67, 90, 95, 97, 99, 101,  82, 77, 50, 45, 22};
int shell4[] = {0,  2,  4,  6,  8,  9,  32, 33, 64, 65, 92, 93, 112,  110,  108,  106,  104,  103,  80, 79, 48, 47, 20, 19};



void setup() {
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(9600);
  Serial.println("PlayLight with GPS!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);

  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);



  // put your setup code here, to run once:
  strip.setBrightness(LEDBRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}




// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;
  // writing direct to UDR0 is much much faster than Serial.print
  // but only one character can be written at a time.
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

int brightnessB = 0;
int fadeAmountB = fadeAmount;
int brightnessS1 = 31;
int fadeAmountS1 = fadeAmount;

int brightnessS2 = 0;
int fadeAmountS2 = fadeAmount;
int brightnessS3 = 127;
int fadeAmountS3 = fadeAmount;
int brightnessS4 = 156;
int fadeAmountS4 = fadeAmount;



uint32_t timer = millis();
void loop() {
  // GPS CODE
  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived())
  {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  float xPos = 0;
  float yPos = 0;
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 1000) 
  {
    timer = millis(); // reset the timer

    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 4);
      xPos = (GPS.latitudeDegrees);
      Serial.print(", ");
      Serial.println(GPS.longitudeDegrees, 4);
      yPos = (GPS.longitudeDegrees);

      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }



  // PLAYLIGHT CODE


  // xPos, yPos area detect
  // wien center: 48.20833°, 16.373064°
  if (GPS.fix)
  {

    int wholeRadius = 3000; //in m
    int shells = 5;
    int shellWidth = wholeRadius / shells; // in m

    Serial.print("before: ");
    Serial.println(millis());
    Serial.print("coords: ");
    
    float distance = calcDist(xPos, yPos, 48.20833, 16.373064);
    Serial.print("after: ");
    Serial.println(millis());
    // switch
    Serial.print("Distance: ");
    Serial.println(distance);
    int shell = 0;
    shell = int(distance / shellWidth);
Serial.print("shell: ");
    Serial.println(shell);
    
    switch (shell)
    {
      case 0:
        strip.setPixelColor(base, strip.Color(0, 0, 0, brightnessB));
        brightnessB = brightnessB + fadeAmountB;
        if (brightnessB == 0 || brightnessB == 255) {
          fadeAmountB = -fadeAmountB ;
        }
        break;

      case 1:
        for (int i = 0; i < (sizeof(shell1) / sizeof(int)); i++)
        {
          strip.setPixelColor(shell1[i], strip.Color(0, 0, 0, brightnessS1));

        }
        brightnessS1 = brightnessS1 + fadeAmountS1;
        if (brightnessS1 == 0 || brightnessS1 == 255) {
          fadeAmountS1 = -fadeAmountS1 ;
        }
        break;

      case 2:
        for (int i = 0; i < (sizeof(shell2) / sizeof(int)); i++)
        {
          strip.setPixelColor(shell2[i], strip.Color(0, 0, 0, brightnessS2));

        }
        brightnessS2 = brightnessS2 + fadeAmountS2;
        if (brightnessS2 == 0 || brightnessS2 == 255) {
          fadeAmountS2 = -fadeAmountS2 ;
        }
        Serial.print("\nBrightness: ");
        Serial.println(brightnessS2);
        break;

      case 3:
        for (int i = 0; i < (sizeof(shell3) / sizeof(int)); i++)
        {
          strip.setPixelColor(shell3[i], strip.Color(brightnessS3, brightnessS3 / 2, 0, brightnessS3));

        }
        brightnessS3 = brightnessS3 + fadeAmountS3;
        if (brightnessS3 == 0 || brightnessS3 == 255) {
          fadeAmountS3 = -fadeAmountS3 ;
        }
        break;

      case 4:
        for (int i = 0; i < (sizeof(shell4) / sizeof(int)); i++)
        {
          strip.setPixelColor(shell4[i], strip.Color(0, brightnessS4, brightnessS4, brightnessS4));

        }
        brightnessS4 = brightnessS4 + fadeAmountS4;
        if (brightnessS4 == 0 || brightnessS4 == 255) {
          fadeAmountS4 = -fadeAmountS4 ;
        }
        break;
    }
    strip.show();


  }
  else
  {
    int del = 1;
    //colorWipe(strip.Color(255, 0, 0, 0), del);
    //colorWipe(strip.Color(0, 0, 0, 0), del);
    //colorWipe(strip.Color(0, 0, 0, 255), );
    //colorWipe(strip.Color(0, 0, 0, 0), );
    //colorWipe(strip.Color(255, 0, 255, 0), del);
    //colorWipe(strip.Color(0, 0, 0, 0), del);
  }















  /*
    strip.show();



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
      strip.setPixelColor(i, strip.Color(brightness, 0, brightness, 0 ));

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

  //Serial.println("end loop");
}




uint16_t wipeCounter = 0;

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, bool _enabled) 
{
  if(_enabled)
  {
    strip.setPixelColor(mappedSpiral(wipeCounter), c);
    wipeCounter++;
    strip.show();
  }
  if (wipeCounter >= HEX_LEDS)
  {
    wipeCounter = 0;
    _enabled = false;
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


float calcDist(float _x1, float _y1, float _x2, float _y2)
{
  float distanceG = sqrt( sq(_y2 - _y1) + sq(_x2 - _x1) );
  return (distanceG / 0.00001);
}

