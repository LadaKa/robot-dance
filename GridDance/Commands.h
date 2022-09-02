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

    Commands() {};

    void addCommand(Command command)
    {
      commandsArray[last + 1] = command;
      last++;
    }

    bool hasNextCommand()
    {
      //Serial.println("Has: ");
      //Serial.println(last);
     // Serial.println(first);
     // Serial.println(last > -1 && first <= last);
      return (last > -1 && first <= last);
    }

    Command getNextCommand()
    {
      Command command = commandsArray[first];
      first++;
      return command;
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
