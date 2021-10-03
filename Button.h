#ifndef Button_h
#define Button_h

/*
  Modified Arduino code for detecting the short press.
  https://arduinogetstarted.com/tutorials/arduino-button-long-press-short-press
*/

class Button {
  
public:
  Button(){
  }

  void setPin(int button_pin){
    pin = button_pin;
    pinMode(pin, INPUT_PULLUP);
  }

  bool isPressed(){
    currentState = digitalRead(pin);
    
    if(lastState == HIGH && currentState == LOW)        // button is pressed
      pressedTime = millis();
      
    else if(lastState == LOW && currentState == HIGH) { // button is released
      releasedTime = millis();
      long pressDuration = releasedTime - pressedTime;
      if( pressDuration < SHORT_PRESS_TIME ){
        Serial.println("A short press is detected");
        lastState = currentState;
        return true;
      }   
    }
    lastState = currentState;
    return false;
  }
  
private:
  const int SHORT_PRESS_TIME = 200; // milliseconds
  int pin;
  
  int lastState = HIGH;  
  int currentState;     
  unsigned long pressedTime  = 0;
  unsigned long releasedTime = 0;
};
#endif
