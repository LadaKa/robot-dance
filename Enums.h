#ifndef Enums_h
#define Enums_h

// TODO: grid size as parameter

class Enums
{
	public:
		typedef enum
		{
			Turning,
			Running,
			Waiting,
      ProcessingNextCommand,
			End
		} State;

		typedef enum
		{
			Forward,
			Left,
			Right,
			Backward
		} Direction;

		typedef enum
		{
			North=0,
			East,
			South,
			West
		} Orientation;

		typedef enum
		{
			A, B, C, D, E
		} Position_X;

    Position_X getPositionX_ByUpperChar(char ch){
      return (Position_X)(ch-'A');
    }

    String getPositionX_AsString(Position_X x){
      return String(1, x+'A');
    }

    Orientation getNextOrientation(Orientation orientation)
    {
      return static_cast<Orientation>((orientation + 1) % 4);
    }

    Orientation getPreviousOrientation(Orientation orientation)
    {
      return static_cast<Orientation>((orientation + 4 - 1) % 4);
    }

    Position_X getNextPosition_X(Position_X position_X)
    {
      return static_cast<Position_X>((position_X + 1) % 5);
    }

    Position_X  getPreviousPosition_X (Position_X  position_X)
    {
      return static_cast<Position_X >((position_X + 5 - 1) % 5);
    }

    bool isInTargetPosition(
      Orientation orientation, Position_X x, int y, Position_X target_x, int target_y) {
    switch (orientation) {
        case North:
            Serial.println("N");
            return y == target_y;
        case East:
            Serial.println("E");
            return x == target_x;
        case South:
            Serial.println("S");
            return y == target_y;
        case West:
            Serial.println("W");
            return x == target_x;
      }
   }
    Orientation chooseOrientation_x(Position_X x, Position_X target_x) 
    {
      if (x < target_x) 
      {
        return East;
      }
      return West;
    }

    Orientation chooseOrientation_y(int y, int target_y) {
      if (y < target_y) {
        return North;
      }
      return South;
    }

    Direction chooseDirection(
      Position_X position_x, 
      int position_y,
      Orientation orientation, 
      Orientation target_orientation) 
      {
        if (target_orientation == getNextOrientation(orientation))
           return Right;
        if (target_orientation == getPreviousOrientation(orientation))
           return Left;
        if (isInTheCorner(position_x, position_y))
           return chooseDirectionInTheCorner(position_x, position_y, orientation);
        return Right;
    }

    //TODO: when the sensors are not used during rotation, the turning direction does not matter
    Direction chooseDirectionInTheCorner(
      Position_X position_x, 
      int position_y,
      Orientation orientation) {
        switch (orientation) {
          case North:
            if (position_x == 1)
                return Right;
            break;
          case East:
            if (position_y == 5)
                return Right;
            break;
          case South:
            if (position_x == 5)
                return Right;
            break;
          case West:
            if (position_y == 1)
                return Right;
            break;

      }
      return Left;
    }

    bool isInTheCorner(Position_X position_x, int position_y) {
      if (position_x == 1) {
        if (position_y == 1 || position_y == 5)
            return true;
      }
      else if (position_x == 5) {
        if (position_y == 1 || position_y == 5)
            return true;
      }
      return false;
    }

	private:
	
};

#endif
