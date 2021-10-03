//PROGMEM

#ifndef Choreography_h
#define Choreography_h

class Choreography
{
  public:

    String getDefault()
    { 
      return defaultChoreography_2;
    }


  private:

    const String defaultChoreography = 
"A1N E1 T150 B2 T350 3A T450 4C T567 D2 T700";

const String defaultChoreography_2 = 
"A1N B2 T150 B3 T350";/// 3A T450 4C T567 D2 T700";
};

#endif
