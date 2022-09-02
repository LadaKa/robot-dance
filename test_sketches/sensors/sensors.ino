#include "Sensors.h"

/*
Issues:

1. Serial port

Arduino: 1.8.19 (Linux), Board: "Arduino Uno"
avrdude: ser_open(): can't open device "/dev/ttyACM0": Permission denied
Problem uploading to board.  See https://support.arduino.cc/hc/en-us/sections/360003198300 for suggestions.
processing.app.SerialException: Error opening serial port '/dev/ttyACM0'. Try consulting the documentation at http://playground.arduino.cc/Linux/All#Permission
 at processing.app.Serial.<init>(Serial.java:150)
  ...
  at java.lang.Thread.run(Thread.java:748)
Error opening serial port '/dev/ttyACM0'. Try consulting the documentation at http://playground.arduino.cc/Linux/All#Permission

fixed:
 sudo chmod a+rw /dev/ttyACM0

2. LED
fixed:
  
 */

Sensors sensors;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  Serial.println("set up");
}

void loop() {
  sensors.readSensors();
}
