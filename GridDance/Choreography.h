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

    const String defaultChoreography =  "A1N E1 T150 A1 T350"; 
          
      //    "A1N A2 T150 A3 T300"; 

     //   "A1N B2 T0 B4 T0 B1 T0 C2 T0";// ok
      
             //  wrong turn   
};

#endif
