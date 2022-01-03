#include <stdio.h>
#include <string.h>
#include "TBCommandsParser/TBCommandsParser.h"

char defaultInputText[] = "SW+SETSTATE=ON,\"slow,then fast\",-12800,NOW";

int main(int argCount, char** argv)
{
  char* inputText = defaultInputText;
  
  if( argCount > 0){
    inputText = argv[1];
  }

  printf("Examples of how to use different functions:\n\r\n\r");
  
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

  char* auxPtr;

  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, 1u);
  char myCharArgument[10];
  memset(myCharArgument, 0x00, sizeof(myCharArgument));
  printf("parseCharArrayArg: %s\n\r", TBCommandsParser::parseCharArrayArg(auxPtr, myCharArgument) ? myCharArgument : NULL);

  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, 2u);
  char myCharArgument2[10];
  memset(myCharArgument2, 0x00, sizeof(myCharArgument2));
  printf("parseCharArrayArg: %s\n\r", TBCommandsParser::parseCharArrayArg(auxPtr, myCharArgument2) ? myCharArgument2 : NULL);

  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, 3u);
  long myLongArgument;
  printf("parseLongArg: %ld\n\r", TBCommandsParser::parseLongArg(auxPtr, &myLongArgument) ? myLongArgument : NULL);

  auxPtr = TBCommandsParser::getArgumentNumberPtr(inputText, 4u);
  char myCharArgument3[10];
  memset(myCharArgument3, 0x00, sizeof(myCharArgument3));
  printf("parseCharArrayArg: %s\n\r", TBCommandsParser::parseCharArrayArg(auxPtr, myCharArgument3) ? myCharArgument3 : NULL);
}