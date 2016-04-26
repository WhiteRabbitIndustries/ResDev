#include "lightSpot.h" //include the declaration for this class


 
void lightSpot::lightSpotInitiate(int _id){
  id = _id;
   //gPal = HeatColors_p;
   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, 0xFF0088);
    //pinMode(LED_PIN, OUTPUT); //make that pin an OUTPUT
    ledNumber = random(6,20);
    brightness = random (127,255);
//    if(DEBUG) Serial.print(id);
//    if(DEBUG) Serial.print(": ledNumber: ");
//    if(DEBUG) Serial.print(ledNumber);
//    if(DEBUG) Serial.print(" brightness: ");
//    if(DEBUG) Serial.print(brightness);
//    if(DEBUG) Serial.println();
//    
}

 
//<<const>>
lightSpot::lightSpot(){/*nothing to const*/}

//<<destructor>>
lightSpot::~lightSpot(){/*nothing to destruct*/}


//set color
void lightSpot::setColor(){
        //digitalWrite(LED_PIN,HIGH); //set the pin HIGH and thus turn LED on
}



//update values
void lightSpot::update(){
        
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness < 30)
  {
    //brightness = 0;
    fadeAmount = -fadeAmount ;
  }
  else if (brightness > 255) 
  {
    brightness = 255;
    fadeAmount = -fadeAmount ;
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
