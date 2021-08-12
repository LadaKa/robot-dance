#ifndef Parser_h
#define Parser_h

#include "Enums.h"
#include "Command.h"

class Parser {

public:

  Parser(){
  }

  void setSize(int max_x, int max_y){
    maxX = max_x;
    maxY = max_y;
  }
  
  void readNextCommand(){

    inputAvailable = Serial.available();
    command = readCommandWithCoordinates();
    if ( hasError )
      return;
    int time = readTimeExpression();
    if ( hasError )
      return;   
    command.setTime(time);
    hasCommand = true;
  }

  bool hasNextCommand(){
    return hasCommand;
  }

  Command getNextCommand(){
    hasCommand = false;
    return command;
  }
  
private:

  const int MAX_TIME_DIGITS = 10;  
  
  int maxX = 0;
  int maxY = 0;
  
  bool inputAvailable;
  char lastChar = 0;
  bool hasError = false;
  
  Command command;
  bool hasCommand = false;

  Enums gridEnum;
  
  Command readCommandWithCoordinates(){
    
    char firstCoord;   
    char secondCoord;
    char ch1;
    if (lastChar!=0){
      ch1 = toupper(lastChar);
      lastChar = 0;
    }
    else {
      ch1 = toupper(readNextNonWhitespace());
    }
    char ch2 = toupper(readNextNonWhitespace());
    if ( !inputAvailable ){
      Serial.println("Missing coordinates.");
      hasError = true;
      return;
    }
    return createCommandWithCoordinates(ch1, ch2);
  }

  char readNextNonWhitespace(){
    
    char ch = 32;
    while ((ch == 9) || (ch == 10) || (ch == 13) || (ch == 32)){
      if (Serial.available())
        ch = Serial.read();
      else {
        inputAvailable = false;
        break;
      }   
    }
    
    return ch;
  }

  // time without whitespaces: 't[0-9]+'
  int readTimeExpression(){
    
    char t = readNextNonWhitespace();
    if (tolower(t) != 't') {
      Serial.println("Missing time expression.");
      hasError = true;
      return -1;
    }
    return readDigits();
  }

  int readDigits(){
    char digits[MAX_TIME_DIGITS];
    int index = 0;
    char d;
    // at least one digit needed
    if (Serial.available()){
        d = Serial.read();
        if (!isdigit(d)){
          Serial.println("Missing time value.");
          hasError = true;
          return digits;
        }
        digits[index] = d;
        index++;
    }
    while (Serial.available()){
      d = Serial.read();
      if (isdigit(d)){
        digits[index] = d;
        index++;
      }
      else {
        lastChar = d;
        break;
      }   
    }
    return atoi(digits);
  }

  Command createCommandWithCoordinates(char ch1, char ch2){
    
    Enums::Position_X position_x;
    int position_y;

    Command result;
    
    if (isalpha(ch1) && isdigit(ch2)){
      position_x = gridEnum.getPositionX_ByUpperChar(ch1);
      position_y = ch2 - '0';
      result = Command(position_x, position_y);
    }
    else if (isdigit(ch1) && isalpha(ch2)){
      position_x = gridEnum.getPositionX_ByUpperChar(ch2);
      position_y = ch1 - '0';
      result = Command(position_y, position_x);
    }
    else {
      Serial.println("Invalid coordinates.");
      hasError = true;
      return;
    }
    if (checkCoordinates(position_x, position_y)){
      return result;
    }
    else {
      hasError = true;
      return;
    }
  }

  bool checkCoordinates(Enums::Position_X position_x, int position_y){
    bool valid = true;
    char x = gridEnum.getPositionX_AsChar(position_x);
    if ((x -'A') > maxX){
      valid = false;
      Serial.println("X-coordinate exceeds max value.");
    }
    if (position_y > maxY) {
      valid = false;
      Serial.println("Y-coordinate exceeds max value.");
    }
    return valid;
  }
};

#endif
