/*
  TBCommandsParser.h
  Parse AT like commands.
  Created by Tato Barilatti, January 2, 2021.
*/

#ifndef TBCommandsParser_h
#define TBCommandsParser_h

#define TBCH_INPUT_BUFFER_SIZE 50
#define TBCH_COMMAND_INITIATOR "SW"
#define TBCH_ARGUMENT_SEPARATOR ','

#include <stdio.h>
#include <ctype.h>
#include <cstring>

namespace TBCommandsParser {
    
    bool initiatorMatches(char* buffer);

    char* goToAfterInitiator(char* buffer);

    bool parseCommand(char* buffer, char* commandCharsPtr);

    bool isReadCommand(char* buffer);
    bool hasArguments(char* buffer);
    char* getFirstArgumentPtr(char* buffer);
    bool isTestCommand(char* buffer);
    unsigned int countArguments(char* buffer);
    char* getArgumentNumberPtr(char* buffer, unsigned int argumentNumber);

    bool parseCharArrayArg(char* bufferArgPtr, char* charArrayArgPtr);
    bool parseLongArg(char* bufferArgPtr, long* longArgPtr);
    bool parseUnsignedLongArg(char* bufferArgPtr, unsigned long* unsignedLongArgPtr);
};

#endif