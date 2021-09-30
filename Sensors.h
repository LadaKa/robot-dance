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
		MIDDLE_State = Black;
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
    return MIDDLE;
  }

	bool getAnyOUTER()
	{
    readSensors();
		return (L_OUTER || R_OUTER);
	}

	bool isOnEdge()   
	{
    readSensors();
		return OUTER_State=Edge;      // never used
	}

	bool updateMiddleSensorState()  // never used
	{
    readSensors();
    Serial.println("MiddleSensorState");
    Serial.println(MIDDLE_State);
		if (MIDDLE && !L_INNER && !R_INNER)
		{
			switch (MIDDLE_State)
			{
				case White : 
					MIDDLE_State = Edge;
					return true;	
					//	TODO: update orientation

				case Edge: 
					MIDDLE_State = Black;
					break;
			
				default:
					break;
			}
		}
		else if (!MIDDLE)
		{
			MIDDLE_State = White;
		}
		return false;
	}

	void updateOuterSensorState()
	{
    readSensors();
		switch (OUTER_State) 
		{
			case White:
				OUTER_State = Edge;
				return;
			case Edge:
				OUTER_State = Black;
				return;
			default:
				return;
		}
	}

	bool setOuterSensorStateToWhite()
	{
		OUTER_State = White;
	}

	int L_OUTER, L_INNER, MIDDLE, R_INNER, R_OUTER;
	StateSequence MIDDLE_State, OUTER_State;

	void printSensors()
  {
    // readSensors();
    Serial.print(L_OUTER);
    Serial.print(L_INNER);
    Serial.print(MIDDLE);
    Serial.print(R_INNER);
    Serial.println(R_OUTER);
  }



	
};
#endif
