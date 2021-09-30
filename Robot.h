#ifndef Robot_h
#define Robot_h

#include "Control.h"
#include "Sensors.h"
#include "Enums.h"
#include "Commands.h"
#include "Command.h"

class Robot

// sensor : states onEdge/white/black -no use?
{
private:
  Control control;
  Sensors sensors;

  Enums gridEnum;
  Enums::Direction direction;
  Enums::Orientation orientation;
  Enums::Position_X position_x;
  int position_y; 

  Enums::Position_X  start_position_x;
  int start_position_y;  
  Enums::Orientation start_orientation;

  int target_time = 0;
  Enums::Orientation target_orientation;
  Enums::Position_X target_x;
  int target_y;

  Enums::State state;
  int next_steps;  // TODO: boolean
  int next_turning_steps = 0;
  Commands commands;


public:

  Robot() {};

  Robot(Commands cmds)
  {
  };

  /* methods for basic setting */
  
  void setMotorsAndSpeed(
    int leftPin, int rightPin,
    int minPulse, int maxPulse,
    int speed){
    control.setMotors(leftPin, rightPin, minPulse, maxPulse);
    control.setSpeed(speed, 50); // TODO: param or const
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

  // TODO:  move to some separate class as static string
  void setDefaultChoreo(){
    
    setStartPosition(
      gridEnum.getPositionX_ByUpperChar('A'), 1, gridEnum.getOrientation_ByChar('N'));
   // commands.addCommand(Command(gridEnum.getPositionX_ByUpperChar('A'), 2, 0));  // what if equals?
   // commands.addCommand(Command(gridEnum.getPositionX_ByUpperChar('A'), 3, 0));
   // commands.addCommand(Command(gridEnum.getPositionX_ByUpperChar('A'), 4, 0));  
    commands.addCommand(Command(gridEnum.getPositionX_ByUpperChar('A'), 2, 0));  
    commands.addCommand(Command(gridEnum.getPositionX_ByUpperChar('B'), 2, 0));  
  }

  void setStartPosition(
    Enums::Position_X  x,
    int y,
    Enums::Orientation o)
    {
      start_position_x = x;
      start_position_y = y;
      start_orientation = orientation;
      position_x = x;
      position_y = y;
      orientation = o;
      direction = gridEnum.Forward;
    }

  void goToStartPosition()
    {
      commands.reset(Command(start_position_x, start_position_y, 0));
      
    }


  /* movement by state */

  // state Running
  void goToNextCrossing() {

    while (!sensors.getAnyOUTER())
    {
      if (sensors.MIDDLE)
      {
        moveCloserToLine();
      }
      else 
      {
        control.move(direction);
      }
    }
    updatePosition();
    checkPosition();
    delay(300);
  }

  // state Turning
  void turn()
  {
    while (sensors.getMiddle())
    {
      control.move(direction);
    }
    while (!sensors.getMiddle())
    {
      control.move(direction);
    }
    updateOrientation();
    checkTargetOrientation();
  }

  void checkTargetOrientation()
  {   
    if (orientation == target_orientation)
    {
        direction = gridEnum.Forward;
        state = gridEnum.Running;
    } 
  }
    


  // state Waiting
  void wait() {
    if ((millis()/1000) >= target_time){
      state = gridEnum.ProcessingNextCommand;
    }
    else 
    {
      control.stop();
    }
  }

  void end() {
    control.stop();
    state = gridEnum.End;
  }

  // process next pre-set choreo command
  void processNextDefaultCommand(){
    if (commands.hasNextCommand()) {   
        Command cmd = commands.getNextCommand();
        processNextCommand(cmd);
      }
      else {
        end();
      }
  };

  // process next choreo command
  void processNextCommand(Command cmd)
  {

    target_x = cmd.x;
    target_y = cmd.y;
    target_time = cmd.time;

    if (( position_x == target_x) && ( position_y == target_y)){
      state = Enums::Waiting;
      control.stop();
    }
    target_orientation = getTargetOrientation(cmd.orderedCoordinates);
    setStateByOrientation();
  }

   Enums::Orientation getTargetOrientation(bool orderedCoordinates)
   {  
    if (orderedCoordinates){    // A1
      if ( position_x != target_x)
        return gridEnum.chooseOrientation_x(position_x, target_x);
      else 
        return gridEnum.chooseOrientation_y(position_y, target_y);
    }
    
    else {                      // 1A
      if ( position_y != target_y)
        return gridEnum.chooseOrientation_y(position_y, target_y);
      else 
        return gridEnum.chooseOrientation_x(position_x, target_x);
    }
   }



private:

  void checkPosition() {
    
      if (target_x == position_x)       
      {
        if (target_y == position_y)
        {
          state = Enums::Waiting;
          control.stop();  // will be stopped anyway
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

  void setStateByOrientation() {

    printInfoAndPose("setStateByOrientation");
    if (orientation == target_orientation)
    {
      state = gridEnum.Running;
    }
    else
    {
      direction = gridEnum.chooseDirection(
        position_x, position_y, orientation, target_orientation);
      state = gridEnum.Turning;
      next_turning_steps == 0; 
    }
  }

 /* void updateStateBeforeCrossing() {
    sensors.updateOuterSensorState();  // any use?
    updatePosition();
    //next_steps = 1;
    delay(300);
  }*/

  void moveCloserToLine() {
    //sensors.OUTER_State = sensors.White;
    
    if (sensors.L_INNER && !sensors.R_INNER) {
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

  void printInfoAndPose(String info)
  {
    printPose("    current: ", position_x, position_y, orientation);
    printPose("target: ", target_x, target_y, target_orientation);
    Serial.println("[" + info + "]");
  }

  void printPose(
    String label,
    Enums::Position_X pos_x, 
    int pos_y, 
    Enums::Orientation orient)
  {
    Serial.print(label);
    Serial.print(gridEnum.getPositionX_AsChar(pos_x));
    Serial.print(pos_y);
    Serial.print(":");
    Serial.print(orient);
    Serial.print(orient+" ");
  }
  

};

#endif#
