#ifndef Commands_h
#define Commands_h

#include "Enums.h"
#include "Command.h"

class Commands
{
public:
	Enums::Position_X x;
	int y;
	int time;

	Command commandsArray[10];  

  Commands(){};

  Commands(int commandsCount)  //!!
  {
   // commandsTotalCount = commandsCount;
  }
 
	void addCommand(Command command)
	{   
		commandsArray[last+1] = command;
    last++;
   Serial.print(commandsArray[0].x);
  Serial.println(commandsArray[0].y);
  Serial.print(commandsArray[1].x);
  Serial.println(commandsArray[1].y);
  Serial.print(commandsArray[2].x);
  Serial.println(commandsArray[2].y);
  Serial.println(last);
  Serial.println();
	}

	bool hasNextCommand()
	{
    Serial.println("Has ");
    Serial.print(commandsArray[0].x);
  Serial.println(commandsArray[0].y);
  Serial.print(commandsArray[1].x);
  Serial.println(commandsArray[1].y);
  Serial.print(commandsArray[2].x);
  Serial.println(commandsArray[2].y);
  Serial.println(last);
  Serial.println();
    Serial.println(last);
		return (last > -1 && first <= last);
	}

	Command getNextCommand()
	{
		Command command = commandsArray[first];
		first++;
    return command;
	}

private:
	int first = 0;
	int last = -1;
};
#endif
