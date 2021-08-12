#ifndef Robot_h
#define Robot_h

#include "Control.h"
#include "Sensors.h"
#include "Enums.h"
#include "Commands.h"
#include "Command.h"

class Robot
{
private:
  Control control;
  Sensors sensors;

  Enums gridEnum;
  Enums::Direction direction;
  Enums::Orientation orientation;
  Enums::Position_X position_x;
  int position_y; 

  int target_time = 0;
  Enums::Orientation target_orientation;
  Enums::Position_X target_x;
  int target_y;

  Enums::State state;
  int next_steps;
  int next_turning_steps = 0;
  int turning_steps_count = 50;
  Commands commands;


public:

  Robot() {};

  Robot(Commands cmds)
  {
  };

  void setMotorsAndSpeed(
    int leftPin, int rightPin,
    int minPulse, int maxPulse,
    int speed){
    control.setMotors(leftPin, rightPin, minPulse, maxPulse);
    control.setSpeed(speed, speed);
  }

  

  void setPose(Enums::Position_X x, int y, Enums::Orientation o, Enums::Direction dir)
  {
    position_x = x;
    position_y = y;
    orientation = o;
    direction = dir;
  }

  void setState(Enums::State s) //!!
  {
    state = s;
  }

  void setCommands(Commands cmds)
  {
    commands = cmds;
  }
  

  Enums::State getState()
  {
    return state;
  }



  void updateAndGoStraight() {
    sensors.readSensors();
    if (next_steps > 0) {
      goToCrossing();
    }
    else if (sensors.getAnyOUTER()) {
      updateStateBeforeCrossing();
    }
    else if (sensors.MIDDLE) {
      moveCloserToLine();
      return;
    }
    control.move(direction);
  }

  void turn()
  {
    if (next_turning_steps == 0)
    {
      next_turning_steps = turning_steps_count;
    }
    else if (next_turning_steps > 1)
    {
      next_turning_steps--;
    }
    else
    {
      updateOrientation();
      if (orientation == target_orientation)
      {
        direction = gridEnum.Forward;
        state = gridEnum.Running;
        return;
      }
      next_turning_steps = turning_steps_count;
    }
    control.move(direction);
  }

  void updateAndTurn()
  {
    sensors.readSensors();
    if (sensors.updateMiddleSensorState())
    {
      updateOrientation();
      if (orientation == target_orientation)
      {
        direction = gridEnum.Forward;
        state = gridEnum.Running;
        return;
      }
    }
    control.move(direction);
  }

  void wait(int time) {
    if (time >= target_time){
      state = gridEnum.ProcessingNextCommand;
    }
  }

  void end() {
    control.stop();
    state = gridEnum.End;
  }


  void processNextDefaultCommand(){
    if (commands.hasNextCommand()) {   
        Command cmd = commands.getNextCommand();
        processNextCommand(cmd);
      }
      else {
        end();
      }
  };

  void processNextCommand(Command cmd)
  {

    target_x = cmd.x;
    target_y = cmd.y;
    target_time = cmd.time;

    Serial.print("Current position: ");
    Serial.print(gridEnum.getPositionX_AsChar(position_x));
    Serial.println(position_y);
    Serial.print("Target position: ");
    Serial.print(gridEnum.getPositionX_AsChar(target_x));
    Serial.println(target_y);

    if (( position_x == target_x) && ( position_y == target_y)){
      state = Enums::Waiting;
      control.stop();
    }
    
    if (cmd.orderedCoordinates){
      if ( position_x != target_x)
        target_orientation = gridEnum.chooseOrientation_x(position_x, target_x);
      else 
        target_orientation = gridEnum.chooseOrientation_y(position_y, target_y);
    }
    else {
      if ( position_y != target_y)
        target_orientation = gridEnum.chooseOrientation_y(position_y, target_y);
      else 
        target_orientation = gridEnum.chooseOrientation_x(position_x, target_x);
    }
    setStateByOrientation();
  }

  void setDefaultChoreo(){
    commands.addCommand(Command(gridEnum.C, 1, 0));
    commands.addCommand(Command(gridEnum.D, 1, 0));
    commands.addCommand(Command(gridEnum.D, 2, 0));
  }

private:

  void goToCrossing() {
    
    next_steps = next_steps - 1;
    control.move(direction);
    if (next_steps == 0)
    {
      Serial.print("Before crossing; last position: ");
      Serial.print(gridEnum.getPositionX_AsChar(position_x));
      Serial.println(position_y);
      
      sensors.setOuterSensorStateToWhite();
      if (target_x == position_x)       
      {
        if (target_y == position_y)
        {
          Serial.println("Waiting");
          state = Enums::Waiting;
          control.stop();
        }
        else
        {
          target_orientation = gridEnum.chooseOrientation_y(position_y, target_y);
          setStateByOrientation();
        }
      }
      else
      {
        target_orientation = gridEnum.chooseOrientation_x(position_x, target_x);
        setStateByOrientation();
      }
    }
  }

  void setStateByOrientation() {
    if (orientation == target_orientation)
    {
      state = gridEnum.Running;
    }
    else
    {
      direction = gridEnum.chooseDirection(
        position_x, position_y, orientation, target_orientation);
      state = gridEnum.Turning;
    }
  }

  void updateStateBeforeCrossing() {
    sensors.updateOuterSensorState();
    if (sensors.isOnEdge())
    {
      updatePosition();
      next_steps = 5;
    }
  }

  void moveCloserToLine() {
    sensors.OUTER_State = sensors.White;
    if (sensors.L_INNER) {
      control.move(gridEnum.Left);
    }
    else if (sensors.R_INNER) {
      control.move(gridEnum.Right);
    }
    else {
      control.move(gridEnum.Forward);
    }
  }

  void updatePosition() {
    switch (orientation) {
    case gridEnum.North:
      position_y = position_y + 1;
      break;
    case gridEnum.West:
      position_x = gridEnum.getPreviousPosition_X(position_x);
      break;
    case gridEnum.South:
      position_y = position_y - 1;
      break;
    case gridEnum.East:
      position_x = gridEnum.getNextPosition_X(position_x);
      break;
    }
  }

  void updateOrientation() 
  {
    switch (direction) {
    case  gridEnum.Left:
      orientation = gridEnum.getPreviousOrientation(orientation);
      break;
    case gridEnum.Right:
      orientation = gridEnum.getNextOrientation(orientation);
      break;
    }
  }

};

#endif#
