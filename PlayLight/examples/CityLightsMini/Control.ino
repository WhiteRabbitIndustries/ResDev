// button inputs and switch mechanisms.


//states 

const int stateButton0 = 6;   
const int stateButton1 = 7;   
const int stateButton2 = 8;
const int stateButton3 = 9;
const int stateButton4 = 10;

//int state = 0;         // variable for reading the pushbutton status

void setButtons()
{  // initialize the pushbutton pin as an input:
    pinMode(stateButton0, INPUT);
  pinMode(stateButton1, INPUT);
  pinMode(stateButton2, INPUT);
  pinMode(stateButton3, INPUT);
  pinMode(stateButton4, INPUT);
  }

// check buttons
void checkButtons()
{
  
   //buttonState = digitalRead(buttonPin);
  if (digitalRead(stateButton0) == LOW) 
  {
      state = 0;
      if(DEBUG) 
      {
        Serial.print("state: ");
        Serial.println(state);
        
      }
  }
  else if (digitalRead(stateButton1) == LOW) 
  {
      state = 1;
      if(DEBUG) 
      {
        Serial.print("state: ");
        Serial.println(state);
        
      }
  }
  else if (digitalRead(stateButton2) == LOW) 
  {
      state = 2;
            if(DEBUG) 
      {
        Serial.print("state: ");
        Serial.println(state);
        
      }
  }
  else if (digitalRead(stateButton3) == LOW) 
  {
      state = 3;
           if(DEBUG) 
      {
        Serial.print("state: ");
        Serial.println(state);
        
      }
  }  
    else if (digitalRead(stateButton4) == LOW) 
  {
      state = 4;
      state4Click = millis();
           if(DEBUG) 
      {
        Serial.print("millis: ");
        Serial.print(state4Click);
        
        Serial.print(" state: ");
        Serial.println(state);
        
      }
  } 
  
  }
// check 



