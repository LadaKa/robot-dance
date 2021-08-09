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

	Command commandsArray[10];  // TODO: list in arduino?

  Commands(){}
 
	Commands(int commandsCount)
	{
		commandsTotalCount = commandsCount;
	}

	void addCommand(Command command)
	{
		commandsArray[index] = command;
   Serial.print(commandsArray[0].x);
  Serial.println(commandsArray[0].y);
  Serial.print(commandsArray[1].x);
  Serial.println(commandsArray[1].y);
  Serial.print(commandsArray[2].x);
  Serial.println(commandsArray[2].y);
  Serial.println();
	}

	bool hasNextCommand()
	{
		return (index == commandsTotalCount);
	}

	Command getNextCommand()
	{
		Command command = commandsArray[index];
		index++;
    return command;
	}

private:
	int currentCount = 0;
	int index = 0;
	int commandsTotalCount;
};
#endif
