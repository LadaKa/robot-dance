#ifndef Control_h
#define Control_h

#include "motor.h"

/* Control of robot left and right motors */

class Control     
{
  public:

    Control()
    {};

    void setMotors(int leftPin, int rightPin, int minPulse, int maxPulse)
    {
      int centerPulse = (minPulse + maxPulse) / 2;

      _leftMotor.attach(leftPin, minPulse, maxPulse);
      _leftMotor.setCenterPulseAndDirection(centerPulse, false);

      _rightMotor.attach(rightPin, minPulse, maxPulse);
      _rightMotor.setCenterPulseAndDirection(centerPulse, true);
    }

    void setSpeed(int speedParam, int turnSpeed)
    {
      speed = speedParam;
      rotationSpeed = turnSpeed;
      insideTurnSpeed = turnSpeed/2;
      outsideTurnSpeed = turnSpeed;
    }

    void move(Enums::Direction direction) {
      lastDirection = direction;
      switch (direction) {
        case gridEnum.Forward:
          moveForward();
          break;
        case gridEnum.Left:
          turnLeft();   // pivot turn
          break;
        case gridEnum.Right:
          turnRight();  // pivot turn
          break;
        default:
          break;
      }
    }

    void rotate(Enums::Direction direction) {
      lastDirection = direction;
      switch (direction) {
        case gridEnum.Left:
          rotateLeft();
          break;
        case gridEnum.Right:
          rotateRight();
          break;
        default:
          break;
      }
    }

    // correction movement - doesn't work properly
    void moveInOppositeDirection()
    {
    
      switch (lastDirection) {
        case gridEnum.Left:
          move(gridEnum.Right);
          break;
        case gridEnum.Right:
          move(gridEnum.Left);
          break;
        default:
          move(gridEnum.Right);
          break;
      }
      delay(150);
    }

    void stop()
    {
      _leftMotor.stop();
      _rightMotor.stop();
    }

  private:

    void moveForward()
    {
      _leftMotor.go(speed);
      _rightMotor.go(speed);
    }

    void turnLeft()
    {
      _leftMotor.go(insideTurnSpeed);
      _rightMotor.go(outsideTurnSpeed);
    }
    
    void turnRight()
    {
      _leftMotor.go(outsideTurnSpeed);
      _rightMotor.go(insideTurnSpeed);
    }
    
    void rotateLeft()
    {
      _leftMotor.go( -rotationSpeed);
      _rightMotor.go( rotationSpeed);
    }

    void rotateRight()
    {
      _leftMotor.go(  rotationSpeed);
      _rightMotor.go(-rotationSpeed);
    }


  
    int speed;
    int rotationSpeed;         
    int insideTurnSpeed;
    int outsideTurnSpeed;
    Motor _leftMotor, _rightMotor;

    Enums gridEnum;
    Enums::Direction lastDirection = gridEnum.Forward;
};

#endif
