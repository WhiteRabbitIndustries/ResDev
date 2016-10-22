#include "lightSpot.h" //include the declaration for this class


//<<const>>
lightSpot::lightSpot()
{
  brightness = 255;
  pixelMode = off;
  gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow);
}

//<<destructor>>
lightSpot::~lightSpot() {
  /*nothing to destruct*/
}

void lightSpot::initiate(int _pixelNumber, int _ledNumber, CRGB _ledStrip[], int stripLength)
{
  pixelNumber = _pixelNumber;
  ledNumber = _ledNumber;
  ledStrip = _ledStrip;
}



bool lightSpot::isOff()
{
  if (pixelMode == off) return true;
  else return false;
}

bool lightSpot::isOn()
{
  if (pixelMode != off) return true;
  else return false;
}

//ACTIONS
void lightSpot::activate()
{
  pixelMode = active;
}

void lightSpot::reach()
{
  pixelMode = passive;
  //color.fadeLightBy( 7 );
}
void lightSpot::deactivate()
{
  pixelMode = off;
  color.fadeToBlackBy( 64 );
}

void lightSpot::setBrightness(int _brightness)
{
  brightness = _brightness;
}


//update values
void lightSpot::update()
{
  // state machine here, based on mode.
  switch (pixelMode)
  {
    case off:
      // hex light
      //color = CRGB::Black;
      fadeToBlack();
      break;

    case active:
      // flicker
      flicker();
      break;

    case passive:
      // sleep
      sleepBreath();
      break;
    default:

      break;

  }

  ledStrip[ledNumber] = color;
}





// Light effects

void lightSpot::flicker()
{
  random16_add_entropy( random());

  // Array of temperature readings at each simulation cell
  static byte heat;

  // Step 1.  Cool down every cell a little

  heat = qsub8( heat,  random8(0, ((cooling * 10)) + 2));


  //    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  //    for( int k= NUM_LEDS - 1; k >= 2; k--) {
  //      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  //    }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < sparking )
  {
    //int y = random8();
    //int y = SPARKCELL;

    heat = qadd8( heat, random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  // Scale the heat value from 0-255 down to 0-240
  // for best results with color palettes.
  byte colorindex = scale8( heat, 240);
  color = ColorFromPalette( gPal, colorindex);

}




void lightSpot::sleepBreath()
{

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > fadeDelay)
  {
    previousMillis = currentMillis;

    fadeBrightness = fadeBrightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (fadeBrightness < minFadeBrightness)
    {
      fadeBrightness = minFadeBrightness;
      fadeAmount = -fadeAmount ;
    }
    else if (fadeBrightness > maxFadeBrightness)
    {
      fadeBrightness = maxFadeBrightness;
      fadeAmount = -fadeAmount ;
    }
    // scaling based on brightness:
    //float temp = float(fadeBrightness) / float(255) * float(brightness);
    color = ColorFromPalette( gPal,fadeBrightness);
    color.fadeLightBy( 7 );
  }
}

void lightSpot::fadeToBlack()
{
  //if (color != CRGB::Black)
  {
    color.fadeToBlackBy( 10 );
  }
}
//
//
////turn the LED on
//void lightSpot::on(){
//        digitalWrite(LED_PIN,HIGH); //set the pin HIGH and thus turn LED on
//}
//
////turn the LED off
//void lightSpot::off(){
//        digitalWrite(LED_PIN,LOW); //set the pin LOW and thus turn LED off
//}
//
////blink the LED in a period equal to paramterer -time.
//void lightSpot::blink(int time){
//        on();                   //turn LED on
//        delay(time/2);  //wait half of the wanted period
//        off();                  //turn LED off
//        delay(time/2);  //wait the last half of the wanted period
//}
