
#define MIN_PULSE  500
#define MAX_PULSE 2500
#define LEFT_PIN  12
#define RIGHT_PIN 13
#define BUTTON_PIN 2

#define LED 11

#define SPEED 200
#define TURN_SPEED 30

/*
 *  Test of button:
 *  works fine.
 */
int lastButtonPressTime = 0;
volatile bool buttonPressed = false;

void setup() {
  //  button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPressed, FALLING);

  //  led setup
  pinMode(LED, OUTPUT);

}

// opakujici se smyčka
void loop() {
  if (true)//buttonPressed)
  {
    buttonPressed = false;
    digitalWrite(LED, HIGH); //zapnutí led
    delay(1000); // po dobu jedné sekundy
    digitalWrite(LED, LOW); // vypnuti LED
  }
}


// button press handler (using interrupt)
void onButtonPressed() {
  int currentTime = millis();
  if ((currentTime - lastButtonPressTime) < 500)
    return;

 // lastButtonPressTime = currentTime;
  buttonPressed = true;
}
