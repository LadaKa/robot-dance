/*
 * GridDance class: 
 *  - setup of grid and robot 
 *  - main loop
 *  - processing of choreography
 *  - button handler
 */
 
#include <Servo.h>
#include "Enums.h"
#include "Robot.h"
#include "Parser.h"
#include "Choreography.h"
#define MIN_PULSE  500
#define MAX_PULSE 2500
#define LEFT_PIN  12
#define RIGHT_PIN 13
#define BUTTON_PIN 2

#define LED 11

#define SPEED 200
#define TURN_SPEED 40

Robot robot;
Enums gridEnum;
Parser parser;
Choreography choreography;

int x_size;
int y_size;

Enums::Direction start_direction;
Enums::State start_state;

int lastButtonPressTime;
volatile bool goingBackToStart = false;


void setup() {

  Serial.begin(9600);         
  
  //  grid properties setup (maximal grid size is 9)
  x_size = 5;
  y_size = 5;
  gridEnum.SetSize(x_size, y_size);

  //  button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN), onButtonPressed, FALLING);
  lastButtonPressTime = 0;
  
  //  led setup
  pinMode(LED, OUTPUT);

  //  robot setup
  robot.setMotorsAndSpeed(
    LEFT_PIN, RIGHT_PIN, MIN_PULSE, MAX_PULSE, SPEED, TURN_SPEED);
  robot.setState(gridEnum.BeforeStart);
}


void loop() {

  if (goingBackToStart){
      robot.setState(gridEnum.GoingBackToStart);
      goingBackToStart = false;
  };

  Enums::State state = robot.getState();
  
  switch (state) {
    
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
void start(int current_time)
{
  String choreo;
  if (Serial.available() > 0) {
    choreo = Serial.readString();         // user input from console
    Serial.println("Processing choreography from console.");
  }
  else {
    choreo = choreography.getDefault();   // pre-set choreography
  }
  processInputCommands(choreo);
  
  robot.setStartTime(current_time);
  robot.setState(gridEnum.ProcessingNextCommand);
  goingBackToStart = false;
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

  robot.flash();
  lastButtonPressTime = currentTime;
  
  switch (robot.getState()) {
    
    case gridEnum.BeforeStart:
      start(currentTime);
      return;
      
    default:
      goingBackToStart = true;
      return;
  }
}
