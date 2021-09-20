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
  int turning_steps_count = 2;
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
      Serial.println(
        "---------------- update state before crossing ----------------");  
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
    /*if (next_turning_steps == 0)      // start of turning
    {
      next_turning_steps = turning_steps_count;
    }
    else if (next_turning_steps > 1)  // turning
    {
      next_turning_steps--;
    }*/
    if (next_turning_steps == 0)      // start of turning
    {
      next_turning_steps = turning_steps_count;
      control.move(direction);
      
    }
    else                              // last turning step

    {
      updateOrientation();
      
      if (orientation == target_orientation)
      {
        direction = gridEnum.Forward;
        state = gridEnum.Running;
        return;
      }
      
      next_turning_steps = turning_steps_count;
      control.move(direction);
    }
    //control.move(direction);
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

    if (( position_x == target_x) && ( position_y == target_y)){
      state = Enums::Waiting;
      control.stop();
    }
    
    if (cmd.orderedCoordinates){    // A1
      if ( position_x != target_x)
        target_orientation = gridEnum.chooseOrientation_x(position_x, target_x);
      else 
        target_orientation = gridEnum.chooseOrientation_y(position_y, target_y);
    }
    
    else {                          // 1A
      if ( position_y != target_y)
        target_orientation = gridEnum.chooseOrientation_y(position_y, target_y);
      else 
        target_orientation = gridEnum.chooseOrientation_x(position_x, target_x);
    }
    setStateByOrientation();
  }

  void setDefaultChoreo(){

    //  right turn A1 -> A2
    //commands.addCommand(Command(gridEnum.getPositionX_ByUpperChar('A'), 2, 0));  

    //  forward
    commands.addCommand(Command(gridEnum.getPositionX_ByUpperChar('C'), 1, 0));

    //  right turn to 2
    commands.addCommand(Command(gridEnum.getPositionX_ByUpperChar('C'), 2, 0));  

  }

private:

  void goToCrossing() {
    
    next_steps = next_steps - 1;
    Serial.println(next_steps );
    control.move(direction);  //  ?
    if (next_steps == 0)
    {
      printInfoAndPose("before crossing");
      
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
    Serial.println("---------------- update state before crossing ----------------");
    sensors.updateOuterSensorState();
    updatePosition();
    next_steps = 1;

  }

  void moveCloserToLine() {
    sensors.OUTER_State = sensors.White;
    
    if (sensors.L_INNER && !sensors.R_INNER) {
      Serial.println("move left");
      control.move(gridEnum.Left);
    }
    else if (sensors.R_INNER) {
      Serial.println("move right");
      control.move(gridEnum.Right);
    }
    else {
      //Serial.println("move forw");
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
