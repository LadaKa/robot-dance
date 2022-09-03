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
      L_INNER_LAST = L_INNER;
      
      R_OUTER = 1 - digitalRead(7);
      R_INNER = 1 - digitalRead(6);
      MIDDLE  = 1 - digitalRead(5);
      L_INNER = 1 - digitalRead(4);
      L_OUTER = 1 - digitalRead(3);
          
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

    bool getAnyINNER()
    {
      readSensors();
      return (L_INNER || MIDDLE || R_INNER);
    }

    bool getLeftINNERValue()
    {
      // don't read again
      return L_INNER;
    }

    int L_OUTER, L_INNER, MIDDLE, R_INNER, R_OUTER;
    int L_INNER_LAST;

    void printSensors()
    {
      Serial.print(L_OUTER);
      Serial.print(L_INNER);
      Serial.print(MIDDLE);
      Serial.print(R_INNER);
      Serial.println(R_OUTER);
    }

};
#endif
