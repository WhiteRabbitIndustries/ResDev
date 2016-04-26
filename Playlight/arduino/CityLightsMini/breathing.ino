
void reflect(int id, boolean level)
{


    //for( int j = 0; j < GAMESPOTCOUNT; j++) 
    //{
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex;
      if(level) colorindex = scale8( gameSpots[id].brightness, 240);
      else 
      {
         colorindex = 4;
      }

      
      CRGB color = ColorFromPalette( gameSpots[id].gPal,colorindex );

      leds[gameSpots[id].ledNumber] = color;

      //gameSpots.
    //}
  
  }



