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
   *   min_pulse ~   0-degree angle on the servo (defaults to  500)     
   *   max_pulse ~ 180-degree angle on the servo (defaults to 2500)
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
    _turnSpeed = turnSpeed;
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

  void moveBackwards() // correction movement
  {
    _leftMotor.go(-15);
    _rightMotor.go(-25);
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
};

#endif
