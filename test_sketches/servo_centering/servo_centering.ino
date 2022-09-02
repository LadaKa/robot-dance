#include <Servo.h>                          
 
Servo servoLeft; 
Servo servoRight;                          

void setup()                                
{
  servoLeft.attach(12);
  servoRight.attach(13); 
  servoLeft.writeMicroseconds(1500);        // 1.5 ms stay still signal                    
  servoRight.writeMicroseconds(1490);
}  
 
void loop()                                 
{    
  /*int x = 40;
  servoLeft.writeMicroseconds(1500-x);        // 1.5 ms stay still signal                    
  servoRight.writeMicroseconds(1500-x);  
  delay(300);    
  servoLeft.writeMicroseconds(1500);        // 1.5 ms stay still signal                    
  servoRight.writeMicroseconds(1500);   
  delay(3000);       */                              
}
