#ifndef Choreography_h
#define Choreography_h

class Choreography
{
  public:

    Choreography()
    {
      value = defaultValue;
    }

    String getValue()
    {
      return value;
    }

    void setValue(String choreographyValue)
    {
      value = choreographyValue;
    }

  private:

  /*  Recorded videos of choreography samples:
   *  
   *  1.  Stairs:
   *      "A1N A2 T30 B2 T60 B3 T90 C3 T120 C4 T150 D4 T180 D5 T210 E5 T240"
   *      
   *  2.  Assignment:
   *      "A1N E1 T150 B2 T350 3A T450 4C T567 D2 T700"
   *      
   *  3.  Short track for return tests (different start position):
   *      "A3S B1 T100" 
   *  
   */

   String value;
   const String defaultValue = "A3S B1 T100";
};

#endif
