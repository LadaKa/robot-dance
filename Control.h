#ifndef Control_h
#define Control_h

#include "motor.h"

class Control
{
public:

  Control()
  {};

  /*
   *   the pulse width [microsec]:
   *   min_pulse ~   0-degree angle on the servo (defaults to  544)
   *   max_pulse ~ 180-degree angle on the servo (defaults to 2400)
   *   
  */
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
  }

  void move(Enums::Direction direction) {
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
    _leftMotor.go(_speed/2 );
    _rightMotor.go(-_speed/2 );
  }

  void turnRight()
  {
    _leftMotor.go(-_speed/2 );
    _rightMotor.go(_speed/2 );
  }


private:
  int _speed;
  int _turnPosSpeed;
  int _turnNegSpeed;
  Motor _leftMotor, _rightMotor;

  Enums gridEnum;
};

#endif
