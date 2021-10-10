#ifndef Sensors_h
#define Sensors_h

/*
    Front IR sensors:

    [L_OUTER]  [L_INNER][MIDDLE][R_INNER]  [R_OUTER]

*/
class Sensors
{
  public:

    void readSensors()
    {
      R_OUTER = 1 - digitalRead(3);
      R_INNER = 1 - digitalRead(4);
      MIDDLE  = 1 - digitalRead(5);
      L_INNER = 1 - digitalRead(6);
      L_OUTER = 1 - digitalRead(7);

    }

    bool getMiddle()
    {
      readSensors();
      return MIDDLE;
    }

    bool getAnyOUTER()
    {
      readSensors();
      return (L_OUTER || R_OUTER);
    }

    int L_OUTER, L_INNER, MIDDLE, R_INNER, R_OUTER;

    void printSensors()
    {
      readSensors();
      Serial.print(L_OUTER);
      Serial.print(L_INNER);
      Serial.print(MIDDLE);
      Serial.print(R_INNER);
      Serial.println(R_OUTER);
    }

};
#endif
