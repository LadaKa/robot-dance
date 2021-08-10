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
   
	}

	bool hasNextCommand()
	{
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
