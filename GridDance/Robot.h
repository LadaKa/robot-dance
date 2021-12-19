#ifndef Robot_h
#define Robot_h

#include "Control.h"
#include "Sensors.h"
#include "Enums.h"
#include "Commands.h"
#include "Command.h"

#define LED 11
/* Robot moving on the grid by given choreography */
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

    Enums::Position_X  start_position_x;
    int start_position_y;
    Enums::Orientation start_orientation;

    int target_time = 0;
    Enums::Orientation target_orientation;
    Enums::Position_X target_x;
    int target_y;

    Enums::State state;
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
      int speed, int turn_speed) {
      control.setMotors(leftPin, rightPin, minPulse, maxPulse);
      control.setSpeed(speed, turn_speed);
    }

    void setState(Enums::State s)
    {
      state = s;
    }

    void setCommands(Commands cmds)
    {
      commands = cmds;
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

    Enums::State getState()
    {
      return state;
    }



    /* movement by state */

    // state Testing
    void test(){
       
      // control.stop();  ok 
     
       control.move(gridEnum.Forward);  // only left
    }
    

    // state Running
    void goToNextCrossing() {

      while (!sensors.getAnyOUTER())
      {
        if (sensors.getAnyINNER())
        {
          followLine();
        }
        else
        {
          control.moveInOppositeDirection();
        }
      }
      control.move(gridEnum.Forward);
      delay(250);
      control.stop();
      flash();
      updatePosition();
      checkPosition();
    }

    // state Turning
    void turn()
    { 
      control.rotate(direction);
      while (sensors.getMiddle()) {}  
      while (!sensors.getMiddle()) {}
      delay(50);
      control.stop();
      flash();
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

      if ((millis() / 100) >= target_time) {
        state = gridEnum.ProcessingNextCommand;
      }
    }


    // state End
    void end() {
      control.stop();
      state = gridEnum.End;
      Serial.println("End");
    }



    // state ProcessingNextCommand
    void processNextCommandIfExists() {
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

      target_orientation = getTargetOrientation(cmd.orderedCoordinates);
      setStateByOrientation();
      direction = gridEnum.chooseDirection(
                    position_x, position_y, orientation, target_orientation);

    }

    Enums::Orientation getTargetOrientation(bool orderedCoordinates)
    {
      if (orderedCoordinates) {   // A1
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


    void goToStartPosition()
    {
      
      commands.reset(Command(start_position_x, start_position_y, 0));
      setState(gridEnum.ProcessingNextCommand);
    }



  private:

    // called only if at least one of inner sensors is seeing the line
    void followLine() {  
      
      if (sensors.getMiddle()) {                   
        control.move(gridEnum.Forward);  
      }
      else if (!sensors.L_INNER && sensors.R_INNER) { 
        control.move(gridEnum.Left);
      }
      else if ( sensors.R_INNER && !sensors.L_INNER){
        control.move(gridEnum.Right);
      }
    }

    void checkPosition() {

      if (target_x == position_x)
      {
        if (target_y == position_y)
        {
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
      printPose("current: ", position_x, position_y, orientation);
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
      Serial.print(orient + " ");
    }

    void flash(){
      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
    }
    

};

#endif#
