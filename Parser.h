#ifndef Parser_h
#define Parser_h

#include "Enums.h"
#include "Command.h"

class Parser {

public:

  Parser(){
    inputAvailable = Serial.available();
  }
  
  // TODO: string for y = 10
  void readNextCommand(){
    char firstCoord;   
    char secondCoord;
    char char1;
    char char2;
    ch1 = toupper(readNextNonWhitespace());
    ch2 = toupper(readNextNonWhitespace());
    if ( !inputAvailable ){
      Serial.println("Missing coordinates.");
      hasError = true;
    }
    else {
      createCommandCoordinates(ch1, ch2);
      if ( hasError )
        return;
      
    }
  }
  
private:

  const int MAX_TIME_DIGITS = 10;  
  

  bool inputAvailable;
  bool hasCommand = false;
  bool hasError = false;
  Command command;
  Enums::Position_X position_x;
  int position_y;


  char readNextNonWhitespace(){
    char ch = 32;
    while ( (ch == 9) || (ch == 10) || (ch == 13) || (ch == 32)){
      if (Serial.available())
        ch = Serial.read();
      else {
        inputAvailable = false;
        break;
      }   
    }
    return ch;
  }

  // time without whitespaces: 't100'
  int readNextTime(){
    
    char t = readNextNonWhitespace();
    if (tolower(t) != 't') {
      Serial.println("Missing time expression.");
      hasError = true;
      return -1;
    }
    
    char digits;
    char d = '0';
   /* while ( isdigit(d) ) {
      if (Serial.available()){
        d = Serial.read();
      }
      else {
        inputAvailable = false;
        break;
      }   
    }*/
    
  }

  void createCommandCoordinates(char ch1, char ch2){
    if (isalpha(ch1) && isdigit(ch2)){
      position_x = Enums::getPositionX_ByUpperChar(ch1);
      position_y = ch2 - '0';
    };
    else if (isdigit(ch1) && isalpha(ch2)){
      position_x = Enums::getPositionX_ByUpperChar(ch2);
      position_y = ch1 - '0';
    }
    else {
      Serial.println(
        "Invalid coordinates: " + ch1 + "," + ch2 + ".");
      hasError = true;
    }
  }

}
