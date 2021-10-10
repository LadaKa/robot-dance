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
      _turnSpeed = turnSpeed;
    }

    void move(Enums::Direction direction) {
      lastDirection = direction;
      switch (direction) {
        case gridEnum.Forward:
          moveForward();
          break;
        case gridEnum.Left:
          turnLeft();
          break;
        case gridEnum.Right:
          turnRight();
          break;
        default:
          break;
      }
    }

    // correction movement
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
          move(gridEnum.Left);
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
      _leftMotor.go(_turnSpeed);
      _rightMotor.go(-_turnSpeed);
    }

    void turnRight()
    {
      _leftMotor.go(-_turnSpeed);
      _rightMotor.go(_turnSpeed);
    }


  private:
    int _speed;
    int _turnSpeed;
    Motor _leftMotor, _rightMotor;

    Enums gridEnum;
    Enums::Direction lastDirection = gridEnum.Forward;
};

#endif
