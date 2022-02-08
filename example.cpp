#include <stdio.h>
#include <string.h>
#include "TBCommandsParser/TBCommandsParser.h"

char defaultInputText[] = "SW+SETSTATE=A,ON,\"slow,then fast\",-12800,,NOW";

int main(int argCount, char** argv)
{
  char* inputText = defaultInputText;
  
  if( argCount > 1){
    inputText = argv[1];
  }
  
  printf("Examples of how to use the different functions:\n\r\n\r");

  printf("COMMAND: %s\n\r\n\r", inputText);

  printf("\n\r## Command evaluation\n\r");
  
  printf("initiatorMatches: %s\n\r", TBCommandsParser::initiatorMatches((char*)inputText) ? "TRUE" : "FALSE");

  if( !TBCommandsParser::initiatorMatches((char*)inputText) ){
    return 0;
  }
  
  printf("goToAfterInitiator: %s\n\r", TBCommandsParser::goToAfterInitiator(inputText));

  char command[10];
  memset(command, 0x00, sizeof(command));
  printf("parseCommand: %s\n\r", TBCommandsParser::parseCommand(inputText, command) ? command : NULL);

  if( !TBCommandsParser::parseCommand(inputText, command) ){
    return 0;
  }

  printf("isReadCommand: %s\n\r", TBCommandsParser::isReadCommand(inputText) ? "TRUE" : "FALSE");

  printf("hasArguments: %s\n\r", TBCommandsParser::hasArguments(inputText) ? "TRUE" : "FALSE");

  printf("getFirstArgumentPtr: %s\n\r", TBCommandsParser::getFirstArgumentPtr(inputText));

  printf("isTestCommand: %s\n\r", TBCommandsParser::isTestCommand(inputText) ? "TRUE" : "FALSE");

  printf("countArguments: %u\n\r", TBCommandsParser::countArguments(inputText));
  
  printf("getArgumentNumberPtr: %s\n\r", TBCommandsParser::getArgumentNumberPtr(inputText, 2u));


  printf("\n\r## Arguments evaluation\n\r");

  char* auxPtr;
  
  unsigned int argumentNumber;

  argumentNumber = 1;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("getArgumentLength(%d): %d\n\r", argumentNumber,TBCommandsParser::getArgumentLength(auxPtr));

  argumentNumber = 2;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("getArgumentLength(%d): %d\n\r", argumentNumber, TBCommandsParser::getArgumentLength(auxPtr));

  argumentNumber = 3;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("getArgumentLength(%d): %d\n\r", argumentNumber, TBCommandsParser::getArgumentLength(auxPtr));

  argumentNumber = 5;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("getArgumentLength(%d): %d\n\r", argumentNumber, TBCommandsParser::getArgumentLength(auxPtr));

  argumentNumber = 5;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("canBeNullArgType(%d): %s\n\r", argumentNumber, TBCommandsParser::canBeNullArgType(auxPtr) ? "TRUE" : "FALSE");

  argumentNumber = 4;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("canBeNullArgType(%d): %s\n\r", argumentNumber, TBCommandsParser::canBeNullArgType(auxPtr) ? "TRUE" : "FALSE");

  argumentNumber = 1;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("canBeCharArgType(%d): %s\n\r", argumentNumber, TBCommandsParser::canBeCharArgType(auxPtr) ? "TRUE" : "FALSE");

  argumentNumber = 2;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("canBeCharArgType(%d): %s\n\r", argumentNumber, TBCommandsParser::canBeCharArgType(auxPtr) ? "TRUE" : "FALSE");

  argumentNumber = 1;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("canBeCharArrayArgType(%d): %s\n\r", argumentNumber, TBCommandsParser::canBeCharArrayArgType(auxPtr) ? "TRUE" : "FALSE");

  argumentNumber = 2;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("canBeCharArrayArgType(%d): %s\n\r", argumentNumber, TBCommandsParser::canBeCharArrayArgType(auxPtr) ? "TRUE" : "FALSE");

  argumentNumber = 2;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("inferArgumentType(%d): %d\n\r", argumentNumber, TBCommandsParser::inferArgumentType(auxPtr));

  printf("inferArgumentType(%d): %d\n\r", argumentNumber, TBCommandsParser::inferArgumentType(inputText, argumentNumber));

  char myCharArgument;
  char myCharArrayArgument[50];
  long myLongArgument;

  argumentNumber = 1;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("parseCharArg(%d): %c\n\r", argumentNumber, TBCommandsParser::parseCharArg(auxPtr, &myCharArgument) ? myCharArgument : ' ');

  argumentNumber = 2;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  memset(myCharArrayArgument, 0x00, sizeof(myCharArrayArgument));
  printf("parseCharArrayArg(%d): %s\n\r", argumentNumber, TBCommandsParser::parseCharArrayArg(auxPtr, myCharArrayArgument) ? myCharArrayArgument : NULL);

  argumentNumber = 3;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  memset(myCharArrayArgument, 0x00, sizeof(myCharArrayArgument));
  printf("parseCharArrayArg(%d): %s\n\r", argumentNumber, TBCommandsParser::parseCharArrayArg(auxPtr, myCharArrayArgument) ? myCharArrayArgument : NULL);

  argumentNumber = 4;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  printf("parseLongArg(%d): %ld\n\r", argumentNumber, TBCommandsParser::parseLongArg(auxPtr, &myLongArgument) ? myLongArgument : NULL);

  argumentNumber = 6;
  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, argumentNumber);
  memset(myCharArrayArgument, 0x00, sizeof(myCharArrayArgument));
  printf("parseCharArrayArg(%d): %s\n\r", argumentNumber, TBCommandsParser::parseCharArrayArg(auxPtr, myCharArrayArgument) ? myCharArrayArgument : NULL);

}