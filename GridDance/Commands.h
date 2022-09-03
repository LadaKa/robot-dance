#ifndef Commands_h
#define Commands_h

#include "Enums.h"
#include "Command.h"

/* 
    Commands contains all steps of given choreography
    and info about last executed step.
*/

class Commands
{
  public:
    Enums::Position_X x;
    int y;
    int time;

    Command commandsArray[10];

    Commands() {};

    void addCommand(Command command)
    {
      commandsArray[last + 1] = command;
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

    void reset()
    {
      first = 0;
      last  = 0;
    }
  
    void reset(Command cmd)
    {
      first = 0;
      last  = 0;
      commandsArray[first] = cmd;
    }

  private:
    int first = 0;
    int last = -1;
};
#endif
