#ifndef Sensors_h
#define Sensors_h

class Sensors
{
	public:

	typedef enum
	{
		White,
		Edge,
		Black
	} StateSequence;



	Sensors()
	{
		MIDDLE_State = Black;  // not used
		OUTER_State  = White;
	};

	void readSensors()
	{
		R_OUTER = 1-digitalRead(3);
		R_INNER = 1-digitalRead(4);
		MIDDLE  = 1-digitalRead(5);
		L_INNER = 1-digitalRead(6);
		L_OUTER = 1-digitalRead(7); 
    
    //printSensors();
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

	int L_OUTER, L_INNER, MIDDLE, R_INNER, R_OUTER;
	StateSequence MIDDLE_State, OUTER_State;

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
