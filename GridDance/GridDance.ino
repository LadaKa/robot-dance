#include <Servo.h>
#include "Enums.h"
#include "Robot.h"
#include "Parser.h"
#include "Choreography.h"
#include "Tests.h"

#define MIN_PULSE  500
#define MAX_PULSE 2500
#define LEFT_PIN  12
#define RIGHT_PIN 13
#define BUTTON_PIN 2

#define LED 11

#define SPEED 200
#define TURN_SPEED 30

Robot robot;
Enums gridEnum;
Parser parser;
Choreography choreography;
Tests tests;

int x_size;
int y_size;

Enums::Direction start_direction;
Enums::State start_state;

int lastButtonPressTime;
bool goingBackToStart = false;


//  debugged: turning and moving forward (speed ~ 200)

//  TODO:     correction while following line   - change rotation to pivot turning
//            rename wheels (L/R) !!!


void setup() {

  Serial.begin(9600);         // TODO: use some other number 

  //  grid properties setup
  x_size = 5;
  y_size = 5;
  gridEnum.SetSize(x_size, y_size);

  //  button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPressed, FALLING);

  //  led setup
  pinMode(LED,OUTPUT);
  
  lastButtonPressTime = 0;

  //  robot setup
  
  robot.setMotorsAndSpeed(
    LEFT_PIN, RIGHT_PIN, MIN_PULSE, MAX_PULSE, SPEED, TURN_SPEED);

 // robot.setState(gridEnum.BeforeStart);
 // Serial.println("Before Start - waiting for button press.");

 robot.setState(gridEnum.Testing);
 Serial.println("TESTING.");
}


void loop() {

  /*
   *  TODO: add this after button and movement tests will be done
   * 
  if (goingBackToStart){
      robot.setState(gridEnum.GoingBackToStart);
      goingBackToStart = false;
  };
  */

  Enums::State state = robot.getState();

  switch (state) {
    case gridEnum.Testing:
      return;
    case gridEnum.BeforeStart:
      return;
    case gridEnum.Turning:
      robot.turn();
      return;
    case gridEnum.Running:
      robot.goToNextCrossing();
      return;
    case gridEnum.Waiting:
      robot.wait();
      return;
    case gridEnum.ProcessingNextCommand:
      robot.processNextCommandIfExists();
      return;
    case gridEnum.GoingBackToStart:
      robot.goToStartPosition();
      return;
    case gridEnum.End:
      return;
  }
}


// input processing and setup of robot's commands
void start()
{
  Serial.println("Start.");
  String choreo;
  if (Serial.available() > 0) {
    choreo = Serial.readString();         // user input from console 
  }
  else {
    choreo = choreography.getDefault();   // pre-set choreography
  }
  processInputCommands(choreo);
  robot.setState(gridEnum.ProcessingNextCommand);
 // robot.setState(gridEnum.Testing);
}


// input processing
void processInputCommands(String choreo)
{ 
  parser.setSize(x_size, y_size);
  parser.readStartPosition(choreo);
  robot.setStartPosition(
    parser.start_position_x,
    parser.start_position_y,
    parser.start_orientation);

  Commands commands;
  while (!parser.endOfInput(choreo))
  {
    parser.readNextCommand(choreo);
    if (parser.hasNextCommand()) {
      commands.addCommand(parser.getNextCommand());
    }
    else {
      parser.printParsedPart(choreo);
      return;
    }
  }
  robot.setCommands(commands);
}


// button press handler (using interrupt)
void onButtonPressed() {
  int currentTime = millis();
  if ((currentTime - lastButtonPressTime) < 1000)
    return;
    
  lastButtonPressTime = currentTime;
  switch (robot.getState()) {
    case gridEnum.BeforeStart:
      start();
      return;
    case gridEnum.End:
      start();
      return;
    case gridEnum.Testing:
      tests.testButton();
      return;
    default:
      goingBackToStart = true;
      return;
  }
}
