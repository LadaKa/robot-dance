#ifndef Enums_h
#define Enums_h

/* Enums for description of position, direction, orientation and state */
class Enums
{
  public:

    int x_size = 5;
    int y_size = 5;

    SetSize(int x, int y)
    {
      x_size = x;
      y_size = y;
    }

    typedef enum
    {
      BeforeStart,
      Turning,
      Running,
      Waiting,
      ProcessingNextCommand,
      GoingBackToStart,
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
      North = 0,
      East,
      South,
      West
    } Orientation;

    typedef enum
    {
      A = 0,
      B = 1,
      C = 2,
      D = 3,
      E = 4,
      F = 5,
      G = 6,
      H = 7,
      I = 8,
      J = 9
    } Position_X;

    Position_X getPositionX_ByUpperChar(char ch) { // toUpper can be used here
      return (Position_X)(ch - 'A');
    }

    Orientation getOrientation_ByChar(char ch)
    {
      switch (toupper(ch))
      {
        case 'N': return North;
        case 'E': return East;
        case 'S': return South;
        default: return West;  // handle error
      }

    }

    char getPositionX_AsChar(Position_X x) {
      return x + 'A';
    }

    int getNextOrientation(Orientation orientation)
    {
      return ((orientation + 1) % 4);
    }

    int getPreviousOrientation(Orientation orientation)
    {
      return ((orientation + 4 - 1) % 4);
    }

    Position_X getNextPosition_X(Position_X position_X)
    {
      return static_cast<Position_X>((position_X + 1) % x_size);
    }

    Position_X  getPreviousPosition_X (Position_X  position_X)
    {
      return static_cast<Position_X >((position_X + x_size - 1) % x_size);
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
      if (isOnBoundary(position_x, position_y, orientation))
        return chooseDirectionOnBoundary(position_x, position_y, orientation);

      if (target_orientation == getNextOrientation(orientation)){
        return Right;
      }
      if (target_orientation == getPreviousOrientation(orientation)){
        return Left;
      }
      return Right;
    }

    Direction chooseDirectionOnBoundary(
      Position_X position_x,
      int position_y,
      Orientation orientation) {

      switch (orientation) {
        case North:
          if (position_x == A)
            return Right;
          break;
        case East:
          if (position_y == y_size)
            return Right;
          break;
        case South:
          if (position_x == x_size - 1)
            return Right;
          break;
        case West:
          if (position_y == 1)
            return Right;
          break;
      }
      return Left;
    }


    bool isOnBoundary(Position_X position_x, int position_y, Orientation orientation) {

      if (orientation == North || orientation == South){
        return (position_x == A
               || position_x == x_size - 1);
      }
      else {
        return (position_y == 1
               || position_y == y_size);
      }
      
      return (
               position_x == A
               || position_x == x_size - 1
               || position_y == 1
               || position_y == y_size);
    }
  private:

};

#endif
