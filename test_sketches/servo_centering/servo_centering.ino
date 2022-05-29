#include <Servo.h>                          
 
Servo servoLeft; 
Servo servoRight;                          

void setup()                                
{
  servoLeft.attach(12);
  servoRight.attach(13); 
  
  servoLeft.writeMicroseconds(1500);        // 1.5 ms stay still signal                    
  servoRight.writeMicroseconds(1500);      
}  
 
void loop()                                 
{                                           
}
