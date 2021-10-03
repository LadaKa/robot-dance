#include <Servo.h>
#include "Enums.h"
#include "Robot.h"
#include "Button.h"
#include "Parser.h"
#include "Choreography.h"

#define MIN_PULSE  500
#define MAX_PULSE 2500
#define LEFT_PIN  13
#define RIGHT_PIN 12
#define BUTTON_PIN 2
/*
 * TODO:  
 *        proper grid structure: enums -> class
 *        debug button press
 *        go to start position on button press
 */

int time; 
Robot robot;
Enums gridEnum;
Button button;
Parser parser;
Choreography choreography;

int x_size;
int y_size;

Enums::Position_X  start_position_x;
int start_position_y;  
Enums::Orientation start_orientation;

Enums::Direction start_direction;
Enums::State start_state;

void setup() {

  time = 0;
  Serial.begin(9600);  // some other number should be used
  
  Serial.println("Setup");
  x_size = 5;
  y_size = 5;
  
  gridEnum.SetSize(x_size, y_size);
  start_direction = gridEnum.Forward;

  button.setPin(BUTTON_PIN);

  robot.setMotorsAndSpeed(
    LEFT_PIN, RIGHT_PIN, MIN_PULSE, MAX_PULSE, 80);   // speed = 80

  robot.setState(
    gridEnum.ProcessingNextCommand);
  // DEBUG button
  // Serial.println("Before Start - waiting for button press."); 
  start(); // DEBUG button 
  
}

void loop() {
  
  time = millis();
  
  Enums::State state = robot.getState();
  //checkButton(state); //  DEBUG button
   
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
    case gridEnum.End:
      return;
  }
}

void checkButton(Enums::State state){
  if (button.isPressed()){
    switch (state) {
      case gridEnum.BeforeStart:
        Serial.println("Button pressed");
        start();
        robot.setState(gridEnum.ProcessingNextCommand);
        return;
      default:  
        robot.goToStartPosition();
        robot.setState(gridEnum.ProcessingNextCommand);
        return;
    }
  }
}

void start()
{
  String choreo;
  if (Serial.available()>0){
    choreo = Serial.readString();
  }
  else {
    choreo = choreography.getDefault();
  }
  processInputCommands(choreo);
  Serial.println("Start");
}


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
     if (parser.hasNextCommand())
        commands.addCommand(parser.getNextCommand());
     else 
        break;
  }
}
