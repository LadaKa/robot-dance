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

#define SPEED 50
#define TURN_SPEED 30

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

  x_size = 5;
  y_size = 5;

  gridEnum.SetSize(x_size, y_size);
  start_direction = gridEnum.Forward;

  button.setPin(BUTTON_PIN);

  robot.setMotorsAndSpeed(
    LEFT_PIN, RIGHT_PIN, MIN_PULSE, MAX_PULSE, SPEED, TURN_SPEED);

  robot.setState(gridEnum.BeforeStart);
  Serial.println("Before Start - waiting for button press.");
}

void loop() {

  checkButton();
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
    case gridEnum.End:
      return;
  }
}

void checkButton() {

  if (button.isPressed()) {
    switch (robot.getState()) {
      case gridEnum.BeforeStart:
        start();
        return;
      case gridEnum.End:
        start();
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
  if (Serial.available() > 0) {
    choreo = Serial.readString();
  }
  else {
    choreo = choreography.getDefault();
  }
  processInputCommands(choreo);
  robot.setState(gridEnum.ProcessingNextCommand);
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
