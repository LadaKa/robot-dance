#include <Servo.h>
#include "Enums.h"
#include "Robot.h"
#include "Button.h"

#define MIN_PULSE  500
#define MAX_PULSE 2500
#define LEFT_PIN  13
#define RIGHT_PIN 12
#define BUTTON_PIN 2

int time;
Robot robot;
Enums gridEnum;
Button button;

//   TODO:  proper grid structure: enums -> class
Enums::Position_X  start_position_x = gridEnum.A;
int start_position_y = 1;   

Enums::Orientation start_orientation = gridEnum.North;
Enums::Direction start_direction = gridEnum.Forward;
Enums::State start_state;

bool defaultChoreo = true;

void setup() {

  time = 0;
  Serial.begin(9600);
  
  start_position_x = gridEnum.A;
  start_position_y = 1;
  start_orientation = gridEnum.North;
  start_direction = gridEnum.Forward;

  robot.setMotorsAndSpeed(
    LEFT_PIN, RIGHT_PIN, MIN_PULSE, MAX_PULSE, 30);
  robot.setPose(
    start_position_x, start_position_y, start_orientation, start_direction);
  robot.setState(
    gridEnum.End);
  button.setPin(BUTTON_PIN);
  if (Serial.available()){
    defaultChoreo = false;
  }
  else {
    robot.setDefaultChoreo();
  }
  Serial.println(defaultChoreo);
  
}

void loop() {
  
  time++;
  Enums::State state = robot.getState();
  checkButton(state);
  
  switch (state) {
    case gridEnum.Turning:
      Serial.println("Turning");
      robot.turn();
      return;
    case gridEnum.Running:
      Serial.println("running");
      robot.updateAndGoStraight();
      return;
    case gridEnum.Waiting:
      Serial.println("waiting");
      robot.wait(time);
      return;
    case gridEnum.ProcessingNextCommand: //TODO
      Serial.println("next");
      if (defaultChoreo){
        robot.processNextDefaultCommand();
      }
      else {
        // parser 
      }     
      return;
    case gridEnum.End:
      return;
  }
}

void checkButton(Enums::State state){
  if (button.isPressed()){
    Serial.println("------------------------------pressed");
    switch (state) {
      case gridEnum.End:
        robot.setState(gridEnum.ProcessingNextCommand);
        return;
      default:
        //TODO: go to start
        robot.setState(gridEnum.End);
        return;
    }
  }
}
