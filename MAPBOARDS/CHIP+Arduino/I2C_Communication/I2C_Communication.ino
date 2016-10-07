#include <Wire.h>
#define DEBUG true
// teensy as slave, channel specify. 
void setup() {
  // put your setup code here, to run once:
Wire.begin(9);                // join i2c bus with address #9
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
 
  if (DEBUG) Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void receiveEvent(int howMany)
{
  while (1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
  //////////////////////////
  if(DEBUG) Serial.print("message received: ");
  
//
//  while (Wire.available()) // loop through all but the last
//  {
//
//    incomingByte = Wire.read();
//
//    if ((incomingByte >= '0') && (incomingByte <= '9'))
//    {
//      duration = 10 * duration + incomingByte - '0';
//    }
//    else if (incomingByte == 'A')
//    {
//      armed = true;
//    }
//    else if (incomingByte == 'D')
//    { 
//      armed = false;
//    }
//    //else if (incomingByte == 'T')
//
//
//
//    if (duration != 0 && armed)
//    {
//      messageTime = millis();
//    }
//    else
//    {
//      duration = 0;
//    }
//    /*
//    char c = Wire.read(); // receive byte as a character
//    //Serial.print(c);         // print the character
//    order += c;
//    */
//  }
//   if(DEBUG && armed) Serial.print("armed with duration: ");
//   if(DEBUG) Serial.print(duration);
//   

if(DEBUG) Serial.println();
  /*
  int x = Wire.read();    // receive byte as an integer
  if(DEBUG) Serial.println(x);         // print the integer
  command = x;
  */
//  messageTime = millis();
  
}

void requestEvent()
{
  Wire.write("hello "); // respond with message of 6 bytes
  // as expected by master
}
