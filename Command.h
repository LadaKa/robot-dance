#ifndef Command_h
#define Command_h

#include "Enums.h"
#include "Command.h"
#include "Command.h"

class Command
{
	public:
		Enums::Position_X x;
		int y;
		int time;

    Command(){}

		Command(Enums::Position_X position_x, int position_y, int t)
		{
      x = position_x;
      y = position_y;
			time = t;
		}
};

#endif
