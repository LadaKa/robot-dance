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

    void setSpeed(int speed, int turnSpeed)
    {
      _speed = speed;
      _rotationSpeed = turnSpeed;
      _insideTurnSpeed = turnSpeed/2;
      _outsideTurnSpeed = turnSpeed;
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
    }

    void stop()
    {
      _leftMotor.stop();
      _rightMotor.stop();
    }

  private:

    void moveForward()
    {
      _leftMotor.go(_speed);
      _rightMotor.go(_speed);
    }

    void turnLeft()
    {
      _leftMotor.go(_outsideTurnSpeed);
      _rightMotor.go(_insideTurnSpeed);
    }

    void turnRight()
    {
      _leftMotor.go(_insideTurnSpeed);
      _rightMotor.go(_outsideTurnSpeed);
    }

    void rotateLeft()
    {
      _leftMotor.go(_rotationSpeed);
      _rightMotor.go(-_rotationSpeed);
    }

    void rotateRight()
    {
      _leftMotor.go(-_rotationSpeed);
      _rightMotor.go(_rotationSpeed);
    }


  
    int _speed;
    int _rotationSpeed;         
    int _insideTurnSpeed;
    int _outsideTurnSpeed;
    Motor _leftMotor, _rightMotor;

    Enums gridEnum;
    Enums::Direction lastDirection = gridEnum.Forward;
};

#endif
