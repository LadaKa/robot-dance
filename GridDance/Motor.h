#ifndef Motor_h
#define Motor_h

/*
     the pulse width [microsec]:
     min_pulse ~   0-degree angle on the servo (defaults to  500)
     max_pulse ~ 180-degree angle on the servo (defaults to 2500)

*/

class Motor : public Servo
{
  public:
    Motor(void) {
      _dir = 1;
    }

    void go(int speed)
    {
      writeMicroseconds(_centerPulse + _dir * speed);
    }

    void stop()
    {
      writeMicroseconds(_centerPulse);
    }

    void setCenterPulseAndDirection(int centerPulse, bool right)
    {
      _centerPulse = centerPulse;
      if (right)
        _dir = -1;
      else
        _dir = 1;
    }

  private:
    int _centerPulse;
    int _dir;
};

#endif
