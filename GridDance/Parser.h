#ifndef Parser_h
#define Parser_h

#include "Enums.h"
#include "Command.h"

class Parser {

    /*
       Parser for input choreography in given format.
       E.g.:

       A1N
       E1 T150
       2B T350

    */

  public:

    Parser() {
    }

    Enums gridEnum;

    Enums::Position_X  start_position_x;
    int start_position_y;
    Enums::Orientation start_orientation;


    bool endOfInput(String choreo)
    {
      return currentIndex >= choreo.length();
    }

    void setSize(int max_x, int max_y)
    {
      maxX = max_x;
      maxY = max_y;
    }

    void readStartPosition(String choreo)     // A1N
    {
      inputAvailable = true;
      command = readCommandWithCoordinates(choreo);
      if ( hasError )
        return;
      start_position_x = command.x;
      start_position_y = command.y;
      hasCommand = false;

      char orientation = readNextNonWhitespace(choreo);
      start_orientation = gridEnum.getOrientation_ByChar(orientation); 
      printStartPosition(command, orientation);
    }

    void readNextCommand(String choreo)       // E1 T150
    {
      inputAvailable = currentIndex < choreo.length();
      command = readCommandWithCoordinates(choreo);
      if ( hasError )
        return;
      int time = readTimeExpression(choreo);
      if ( hasError )
        return;
      command.setTime(time);
      // DEBUG
      printCommand(command);
      
      hasCommand = true;
    }

    bool hasNextCommand() {
      return hasCommand;
    }

    Command getNextCommand() {
      hasCommand = false;
      return command;
    }

    bool hasAnyError() {
      return hasError;
    }

    void printParsedPart(String choreo) {
      Serial.println(choreo.substring(0, currentIndex));
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

    int currentIndex = 0;


    Command readCommandWithCoordinates(String choreo) {

      char firstCoord;
      char secondCoord;
      char ch1;
      if (lastChar != 0) {
        ch1 = toupper(lastChar);
        lastChar = 0;
      }
      else {
        ch1 = toupper(readNextNonWhitespace(choreo));
      }
      char ch2 = toupper(readNextNonWhitespace(choreo));
      if ( !inputAvailable ) {
        hasError = true;
        return;
      }
      return createCommandWithCoordinates(ch1, ch2);
    }

    char readNextNonWhitespace(String choreo) {

      char ch = 32;
      while ((ch == 9) || (ch == 10) || (ch == 13) || (ch == 32)
      || (ch == 44) || (ch == 59)) {
        if (currentIndex < choreo.length())
        {
          ch = choreo.charAt(currentIndex);
          currentIndex++;
        }
        else
        {
          inputAvailable = false;
          break;
        }
      }
      return ch;
    }

    // time without whitespaces: 't[0-9]+'
    int readTimeExpression(String choreo) {

      char t = readNextNonWhitespace(choreo);
      if (tolower(t) != 't') {
        hasError = true;
        return -1;
      }
      return readDigits(choreo);
    }

    int readDigits(String choreo) {
      char digits[MAX_TIME_DIGITS];
      int index = 0;
      char d;
      // at least one digit needed
      if (currentIndex < choreo.length()) {
        d = choreo.charAt(currentIndex);
        currentIndex++;
        if (!isdigit(d)) {
          hasError = true;
          return digits;
        }
        digits[index] = d;
        index++;
      }
      while (currentIndex < choreo.length()) {
        d = choreo.charAt(currentIndex);
        currentIndex++;
        if (isdigit(d)) {
          digits[index] = d;
          index++;
        }
        else {
          if (isalpha(d))
          {
            lastChar = d;
          }
          break;
        }
      }
      return atoi(digits);
    }

    Command createCommandWithCoordinates(char ch1, char ch2) {

      Enums::Position_X position_x;
      int position_y;

      Command result;

      if (isalpha(ch1) && isdigit(ch2)) {
        position_x = gridEnum.getPositionX_ByUpperChar(ch1);
        position_y = ch2 - '0';
        result = Command(position_x, position_y);
      }
      else if (isdigit(ch1) && isalpha(ch2)) {
        position_x = gridEnum.getPositionX_ByUpperChar(ch2);
        position_y = ch1 - '0';
        result = Command(position_y, position_x);
      }
      else {
        hasError = true;
        return;
      }
      if (checkCoordinates(position_x, position_y)) {
        return result;
      }
      else {
        hasError = true;
        return;
      }
    }

    bool checkCoordinates(Enums::Position_X position_x, int position_y) {
      bool valid = true;
      char x = gridEnum.getPositionX_AsChar(position_x);
      if ((x - 'A') > maxX - 1) {
        valid = false;
        Serial.println("Coordinate exceeds max value:");
        Serial.println(x);
      }
      else if (position_y > maxY) {
        valid = false;
        Serial.println("Coordinate exceeds max value:");
        Serial.println(position_y);
      }
      return valid;
    }

    void printStartPosition(
        Command command,
        char orientation){
        
      Serial.print(gridEnum.getPositionX_AsChar(command.x));
      Serial.print(command.y);
      Serial.println(orientation);
    }

    void printCommand(Command command){
      if (command.orderedCoordinates){
        Serial.print(gridEnum.getPositionX_AsChar(command.x));
        Serial.print(command.y);
      }
      else {
        Serial.print(command.y);
        Serial.print(gridEnum.getPositionX_AsChar(command.x));
      }
      Serial.print(" T");
      Serial.println(command.time);
    }
};

#endif
