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
  int position_y; // !!

  int target_time = 0;
  Enums::Orientation target_orientation;
  Enums::Position_X target_x;
  int target_y;

  Enums::State state;
  int next_steps;
  int next_turning_steps = 0;
  int turning_steps_count = 30;
  Commands commands;


public:

  Robot() {};

  Robot(Commands cmds)
  {
  };

  void setMotorsAndSpeed(
    int leftPin, int rightPin,
    int minPulse, int maxPulse,
    int speed)
  {
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
    /*if (commands.hasNextCommand()) 
    {
      Command cmd = commands.getNextCommand();
      processNextCommand(cmd);
    }*/
  }
  

  Enums::State getState()
  {
    return state;
  }



  void updateAndGoStraight() {
    Serial.print(position_x);
    Serial.print(" ");
    Serial.println(position_y);
    sensors.readSensors();
    if (next_steps > 0) {
      goToCrossing();
      Serial.println(next_steps);
    }
    else if (sensors.getAnyOUTER()) {
      updateStateBeforeCrossing();
      Serial.println("bef Cr");
    }
    else if (sensors.MIDDLE) {
      Serial.println("Cl");
      moveCloserToLine();
      return;
    }
    control.move(direction);
  }

  void turn()
  {
    Serial.print(" ");
    Serial.println(next_turning_steps);
    Serial.print(orientation );
    Serial.println(target_orientation);
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
    Serial.println("WAIT");
    if (time >= target_time){
      state = gridEnum.ProcessingNextCommand;
    }
    /*if (time >= target_time) {
      if (commands.hasNextCommand()) {   
        Command cmd = commands.getNextCommand();
        processNextCommand(cmd);
      }
      else {
        control.stop();
        state = gridEnum.End;
        Serial.println("YES END"); 
      }
    }*/
  }

  void processNextDefaultCommand(){
    if (commands.hasNextCommand()) {   
        Command cmd = commands.getNextCommand();
        processNextCommand(cmd);
      }
      else {
        control.stop();
        state = gridEnum.End;
        Serial.println("YES END"); 
      }
  };

  void processNextCommand(Command cmd)
  {
    Serial.println("next cmd:");
    Serial.print(cmd.x);
    Serial.println(cmd.y);
    target_x = cmd.x;
    target_y = cmd.y;
    target_time = cmd.time;
    // TODO
    //target_orientation = gridEnum.chooseOrientation_y(position_y, target_y);
    //      setStateByOrientation();
    target_orientation = gridEnum.chooseOrientation_y(position_y, target_y);
    setStateByOrientation();
  }

  void setDefaultChoreo(){
  Serial.println("setting commands");
  //Commands commands(2);
  commands.addCommand(Command(gridEnum.B, 2, 30));
  commands.addCommand(Command(gridEnum.D, 1, 30));
  
  //robot.setCommands(commands);
  
}

private:

  void goToCrossing() {
    next_steps = next_steps - 1;
    control.move(direction);
    if (next_steps == 0)
    {
      sensors.setOuterSensorStateToWhite();
      if (target_x == position_x)        //  y x !!!!!
      {
        if (target_y == position_y)
        {
          state = Enums::Waiting;
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
      next_steps = 1;
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
    case gridEnum.East:
      position_x = gridEnum.getPreviousPosition_X(position_x);
      break;
    case gridEnum.South:
      position_y = position_y - 1;
      break;
    case gridEnum.West:
      position_x = gridEnum.getNextPosition_X(position_x);
      break;
    }
  }

  void updateOrientation() 
  {
    switch (direction) {
    case  gridEnum.Right:
      orientation = gridEnum.getPreviousOrientation(orientation);
      break;
    case gridEnum.Left:
      orientation = gridEnum.getNextOrientation(orientation);
      break;
    }
  }

  
  
};

#endif#
