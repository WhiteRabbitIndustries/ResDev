#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
//SoftwareSerial ss(RXPin, TXPin);
SoftwareSerial ss(RXPin, TXPin);


uint32_t timer = millis();

#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

// different commands to set the update rate from once a second (1 Hz) to 10 times a second (10Hz)
// Note that these only control the rate at which the position is echoed, to actually speed up the
// position fix you must also send one of the position fix rate commands below too.
#define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ  "$PMTK220,10000*2F" // Once every 10 seconds, 100 millihertz.
#define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ  "$PMTK220,5000*1B"  // Once every 5 seconds, 200 millihertz.
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"
// Position fix update rate commands.
#define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ  "$PMTK300,10000,0,0,0,0*2C" // Once every 10 seconds, 100 millihertz.
#define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ  "$PMTK300,5000,0,0,0,0*18"  // Once every 5 seconds, 200 millihertz.
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C"
#define PMTK_API_SET_FIX_CTL_5HZ  "$PMTK300,200,0,0,0,0*2F"
// Can't fix position faster than 5 times a second!


void setup()
{
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);


  ss.begin(GPSBaud);
  // set gps properties:
  ss.write(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  ss.write(PMTK_SET_NMEA_UPDATE_1HZ);
  ss.write(PMTK_API_SET_FIX_CTL_1HZ);
  
  Serial.begin(115200);
  Serial.println(F("WunderKammerWien"));
//  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
//  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
//  Serial.println(F("by Mikal Hart"));
//  Serial.println();

  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
}

#define LOCNUM 11
//48.2117667°, 016.3733194°
// 48.2116250°, 016.3683889°
// 48.2086889°, 016.3725306°
//48.2095556°, 016.3774667°
//48.2092194°, 016.3759306°

// 48.2184361°, 016.3702222°
// 48.2206500°, 016.3640528°
// 48.2152000°, 016.4025500°
// 48.2155667°, 016.3960222°
// 48.2155667°, 016.3960222°

 // 48.2246667°, 016.3662750°

double targetLat[] = {48.2117667,48.2116250, 48.2086889,48.2095556, 48.2092194,
                      48.2184361,48.2206500, 48.2152000,48.2155667, 48.2155667, 48.2246667};
double targetLon[] = {16.3733194,16.3683889, 16.3725306,16.3774667, 16.3759306,
                      16.3702222,16.3640528, 16.4025500,16.3960222, 16.3960222, 016.3662750};

//48.2117139, 16.380369444444444

// latitude: 48°12'44.99"N      48.2124972
// longitude:  16°22'42.82"E    16.3785611
// 4th dec is 11m.
// 48.211528,16.381408 AIL

int bpm = 1;
double distanceM = 1000.0;
void loop()
{
//distanceM = 1000.0;


  // This sketch displays information every time a new sentence is correctly encoded.
  
  while (ss.available() > 0)
  {
    displayInfo();
    if (gps.encode(ss.read()) && gps.location.isValid())
    {
      calcDist();
      displayInfo();
    }
      }
      
    //else
      

  //Serial.print(distanceM);
  //Serial.print("  ");


  //  if (millis() > 5000 && gps.charsProcessed() < 10)
  //  {
  //    Serial.println(F("No GPS detected: check wiring."));
  //    while(true);
  //  }



  bpm = 1;

  if (distanceM > 80)
  {
    digitalWrite(0, HIGH);
    digitalWrite(2, HIGH);
  }
  else if (distanceM <= 80 && distanceM >= 5)
  {
    bpm = map(distanceM, 5, 80, 180, 30);
    //Serial.print(bpm);
    //Serial.print("  ");


    // blinks
    //digitalWrite(0, HIGH);
    if (timer > millis())  timer = millis();
    if (millis() - timer > (60 * 1000 / bpm))
    {
      timer = millis(); // reset the timer

      digitalWrite(0, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(50);              // wait for a second
      digitalWrite(0, HIGH);    // turn the LED off by making the voltage LOW

    }
  digitalWrite(2, HIGH);
  }
  else if (distanceM < 5)
  {
    digitalWrite(0, LOW);
    digitalWrite(2, LOW);
  }






  

}


void calcDist()
{
  
  
  for (int i=0;i<LOCNUM;)
  {
  double distanceG = sqrt( sq(targetLat[i] - gps.location.lat()) + sq(targetLon[i] - gps.location.lng()) );
  double tempDistanceM = distanceG / 0.00001;
  Serial.print(i);
  Serial.print(":");
  Serial.print(tempDistanceM);
  Serial.print("  ");
  
  if(tempDistanceM>100) i++;
  else 
  {
    
    distanceM = tempDistanceM;
    
    Serial.print("in border with ");
    Serial.print(i);
    Serial.print(". selecting disM with: ");
    Serial.print(distanceM);
    Serial.println();
    return;
  }
  
  }
  //Serial.print(distanceG);
  //Serial.print("  ");
  //distanceM = ;
  
  


}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
    
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(gps.satellites.value()); // Number of satellites in use (u32)
Serial.print("  "); // Number of satellites in use (u32)
Serial.print(gps.hdop.value()); // Horizontal Dim. of Precision (100ths-i32)

  Serial.println();
}
