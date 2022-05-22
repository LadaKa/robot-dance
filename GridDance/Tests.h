#ifndef Tests_h
#define Tests_h

#define LED 11


class Tests
{
  public:

    bool button_switch_on;

    void testButton()
    {
      if (button_switch_on)
      {
        digitalWrite(LED, HIGH);
      }
      else
      {
        digitalWrite(LED, LOW);
      }
      button_switch_on = !button_switch_on;
    }
};

#endif
