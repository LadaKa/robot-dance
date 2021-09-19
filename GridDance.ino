#include <Servo.h>
#include "Enums.h"
#include "Robot.h"
#include "Button.h"
#include "Parser.h"

#define MIN_PULSE  544
#define MAX_PULSE 2400
#define LEFT_PIN  13
#define RIGHT_PIN 12
#define BUTTON_PIN 2

/*
 * TODO:  
 *        proper grid structure: enums -> class
 *        grid size as param
 *        start position as parsed param
 *        go to start position button press
 */

int time; // millis() ?
Robot robot;
Enums gridEnum;
Button button;
Parser parser;

int x_size;
int y_size;

// TODO: move start position to robot class
Enums::Position_X  start_position_x;
int start_position_y;  

Enums::Orientation start_orientation;
Enums::Direction start_direction;
Enums::State start_state;

bool defaultChoreo = true;


void setup() {

  time = 0;
  Serial.begin(9600);

  x_size = 5;
  y_size = 5;
  
  start_position_x = gridEnum.A;
  start_position_y = 1;
  start_orientation = gridEnum.East;
  start_direction = gridEnum.Forward;

  button.setPin(BUTTON_PIN);

  robot.setMotorsAndSpeed(
    LEFT_PIN, RIGHT_PIN, MIN_PULSE, MAX_PULSE, 30);
  robot.setPose(
    start_position_x, start_position_y, start_orientation, start_direction);
  robot.setState(
    gridEnum.ProcessingNextCommand);
  //Serial.println("Before Start - waiting for button press.");
  start(); // DEBUG
}

void loop() {

  delay(100);
  time++;
  
  Enums::State state = robot.getState();
  //checkButton(state);
  switch (state) {
    case gridEnum.BeforeStart:
      return;
    case gridEnum.Turning:
      robot.turn();
      return;
    case gridEnum.Running:
      robot.updateAndGoStraight();
      return;
    case gridEnum.Waiting:
      robot.wait(time);
      return;
    case gridEnum.ProcessingNextCommand: 
      Serial.println("ProcessingNextCommand");
      if (defaultChoreo){
        robot.processNextDefaultCommand();
      }
      else {
        processNextSerialInputCommand(); 
      }     
      return;
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
        //TODO: go to start position
        //robot.setState(gridEnum.End);
        robot.end();
        return;
    }
  }
}

void start(){
  if (Serial.available()){
    defaultChoreo = false;
    parser.setSize(x_size, y_size);
  }
  else {
    robot.setDefaultChoreo();
  }
  Serial.println("Start");
}

void processNextSerialInputCommand(){
  parser.readNextCommand();
  if (parser.hasNextCommand()){
    robot.processNextCommand(parser.getNextCommand());
  }
  else {
    robot.end();
  }
}
