/*
  TBCommandsParser.h
  Parse AT like commands.
  Created by Tato Barilatti, January 2, 2021.
*/

#ifndef TBCommandsParser_h
#define TBCommandsParser_h

#define TBCH_COMMAND_INITIATOR "SW"
#define TBCH_ARGUMENT_SEPARATOR ','
#define TBCH_STRING_DELIMITER '"'

#include <stdio.h>
#include <ctype.h>
#include <cstring>

namespace TBCommandsParser {

    enum TBArgumentType {
      nullArg,
      charArg,
      charArrayArg,
      longArg,
      unsignedLongArg,
      floatArg
    };
    
    bool initiatorMatches(char* buffer);

    char* goToAfterInitiator(char* buffer);

    bool parseCommand(char* buffer, char* commandCharsPtr);

    bool isReadCommand(char* buffer);
    bool hasArguments(char* buffer);
    char* getFirstArgumentPtr(char* buffer);
    bool isTestCommand(char* buffer);
    unsigned int countArguments(char* buffer);
    char* getArgumentNumberPtr(char* buffer, unsigned int argumentNumber);

    unsigned int getArgumentLength(char* bufferArgPtr);

    bool canBeNullArgType(char* bufferArgPtr);
    bool canBeCharArgType(char* bufferArgPtr);
    bool canBeCharArrayArgType(char* bufferArgPtr);
    bool canBeLongArgType(char* bufferArgPtr);
    bool canBeUnsignedLongArgType(char* bufferArgPtr);
    bool canBeFloatArgType(char* bufferArgPtr);

    int inferArgumentType(char* bufferArgPtr); // PENDING
    int inferArgumentType(char* buffer, unsigned int argumentNumber);

    bool parseCharArg(char* bufferArgPtr, char* charArgPtr);
    bool parseCharArrayArg(char* bufferArgPtr, char* charArrayArgPtr);
    bool parseLongArg(char* bufferArgPtr, long* longArgPtr);
    bool parseUnsignedLongArg(char* bufferArgPtr, unsigned long* unsignedLongArgPtr);
    // bool parseFloatArg(char* bufferArgPtr, long* longArgPtr); // PENDING
};

#endif