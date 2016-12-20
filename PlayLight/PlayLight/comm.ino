bool firstMessage = false;

void receiveEvent(int howMany)
{
  if (!firstMessage)
  {
    
      playLightState = game;

      firstMessage = true;
    }
  // on receive of I2C message, it means the gameplay routine is running, so switch to game mode from animation.
  
  
  Serial.print("event received with ");
  Serial.println(howMany);
  // PROTOCOL?
  int incomingByte = 0;
  while (Wire.available()) // loop through all but the last
  {
    Serial.println("receiving byte");

    incomingByte = Wire.read();
    if (incomingByte == 'I')
    {
      // initiation maybe? no meaning if all pixels are initiated in the beginning. more memory/space consupmtion,
      // but Teensy3.2 can handle. 
//      
//      int gameLocCount = 0;
//      while (Wire.available())
//      {
//        incomingByte = Wire.read();
//        if ((incomingByte >= '0') && (incomingByte <= '9'))
//        {
//          gameLocCount = 10 * gameLocCount + incomingByte - '0';
//        }
//      }
//      // Create GameSpot array based on received gameLoc count
//      if (locSpotsCount != 0)
//      {
//        delete []locSpots;
//        // also must turn off all leds:
//        for (int l = 0; l<NUM_LEDS;l++)
//        {
//          leds[l] = CRGB::Black;
//          }
//          FastLED.show(); // display this frame
//      }
//      locSpotsCount = gameLocCount;
//      locSpots = new lightSpot [locSpotsCount];
//      /*
//      for(int o = 0; o<locSpotsCount; o++)
//      {
//        locSpots[o] = new lightSpot (0);
//        }
//        */
//      //locSpots = new lightSpot [locSpotsCount];
//      // initalize loc spots:
//      playLightState = game;
//      if (DEBUG)
//      {
//        Serial.print("Game Location Spots initiates with ");
//        Serial.print(locSpotsCount);
//        Serial.print(" spots.");
//        Serial.println();
//      }
    }
    //break;

    else if (incomingByte == 'A')
    {
      int pixelNo = 0;
      while (Wire.available())
      {
        Serial.println("receiving byte");
        incomingByte = Wire.read();
        if ((incomingByte >= '0') && (incomingByte <= '9'))
        {
          pixelNo = 10 * pixelNo + incomingByte - '0';
        }
      }
      //static:
      locSpots[pixelNo].activate();
      if (DEBUG)
      {
        Serial.print("Spot number ");
        Serial.print(pixelNo);
        Serial.println(" is activated.");
      }
    }
    else if (incomingByte == 'R')
    {
      // Deactivate gamespot
      int pixelNo = 0;
      while (Wire.available())
      {
        Serial.println("receiving byte");
        incomingByte = Wire.read();
        if ((incomingByte >= '0') && (incomingByte <= '9'))
        {
          pixelNo = 10 * pixelNo + incomingByte - '0';
        }
      }
      locSpots[pixelNo].reach();
      locSpots[pixelNo].setBrightness(31);
      if (DEBUG)
      {
        Serial.print("Spot number ");
        Serial.print(pixelNo);
        Serial.println(" is reached.");
      }
    }
    else if (incomingByte == 'D')
    {
      // Deactivate gamespot
      int pixelNo = 0;
      while (Wire.available())
      {
        Serial.println("receiving byte");
        incomingByte = Wire.read();
        if ((incomingByte >= '0') && (incomingByte <= '9'))
        {
          pixelNo = 10 * pixelNo + incomingByte - '0';
        }
      }
      locSpots[pixelNo].deactivate();
      
      
      if (DEBUG)
      {
        Serial.print("Spot number ");
        Serial.print(pixelNo);
        Serial.println(" is deactivated.");
      }
    }

  }
  //  extra actions if necessary
}



void requestEvent()
{
  Wire.write("hello!"); // respond with message of 6 bytes
  // as expected by master
}
