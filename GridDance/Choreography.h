#ifndef Choreography_h
#define Choreography_h

class Choreography
{
  public:

    String getDefault()
    {
      return defaultChoreography;
    }

  private:

    const String defaultChoreography =
      //  "A1N A2 T200 A3 T300"; ok

      //    "A1N A2 T0 B2 T0 B1 T0 A1 T0"; ok

      //  "A1N B2 T0 B4 T0 B1 T0 A1 T0"; ok

      "A1N B2 T0 B4 T0 B1 T0 C2 T0";
      
      // "A1N E1 T150 B2 T350";// 3A T450 4C T567 D2 T700";
};

#endif
