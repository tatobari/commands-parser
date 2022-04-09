/*
  TBCommandsParser.h
  Parse AT like commands.
  Created by Tato Barilatti, January 2, 2021.
*/

#include "TBCommandsParser.h"

namespace TBCommandsParser {

  bool initiatorMatches(char* buffer){

    if( buffer == NULL ) return false;

    // Check if the initiator matches.
    if( strncmp(buffer, TBCH_COMMAND_INITIATOR, (unsigned)strlen(TBCH_COMMAND_INITIATOR)) != 0 ){
      return false;
    }

    return true;
  }

  char* goToAfterInitiator(char* buffer){

    if( buffer == NULL ) return NULL;

    return buffer + (unsigned)strlen(TBCH_COMMAND_INITIATOR);
  };

  bool parseCommand(char* buffer, char* commandCharsPtr){

    char* bufferCommandCharsPtr = TBCommandsParser::goToAfterInitiator(buffer);

    if( bufferCommandCharsPtr == NULL ) return false;

    if( *bufferCommandCharsPtr == 0x00 || *bufferCommandCharsPtr == '=' || *bufferCommandCharsPtr == '?' ){
      return false;
    }

    while( *bufferCommandCharsPtr != 0x00 && *bufferCommandCharsPtr != '=' && *bufferCommandCharsPtr != '?' ){
      *commandCharsPtr = *bufferCommandCharsPtr;
      bufferCommandCharsPtr++;
      commandCharsPtr++;
    }

    return true;
  }

  bool isReadCommand(char* buffer){

    char* command = TBCommandsParser::goToAfterInitiator(buffer);

    if( command == NULL ) return false;

    char* questionMark = strchr(command,'?');

    if(questionMark != NULL && *(questionMark-1) != '='){
      return true;
    };

    return false;
  }

  bool hasArguments(char* buffer){

    char* command = TBCommandsParser::goToAfterInitiator(buffer);

    // If the input has no "=" sign, then the command has no arguments.
    if(strchr(command,'=') == NULL){
      return false;
    }

    return true;
  }

  char* getFirstArgumentPtr(char* buffer){

    char* command = TBCommandsParser::goToAfterInitiator(buffer);

    // Find the "=" sign and go to the first character.
    char* argsPtr = strchr(command,'=');

    if(argsPtr == NULL){
      return NULL;
    }

    // Go to argument's first char.
    argsPtr++;

    // If next char is null, then this is the end of the command
    // and it is an invalid command.
    if(*argsPtr == 0x00){
      // TODO: Should save an error code here.
      return NULL;
    }

    // First argument ok.
    return argsPtr;

  }

  bool isTestCommand(char* buffer){

    char* argsPtr = TBCommandsParser::getFirstArgumentPtr(buffer);

    if(argsPtr == NULL){
      return NULL;
    }

    if(*argsPtr == '?' && *(argsPtr+1) == 0x00) return true;

    return false;
  }

  unsigned int countArguments(char* buffer){

    char* argsPtr = TBCommandsParser::getFirstArgumentPtr(buffer);

    // If there is no equal sign, then the count is 0 and the pointer is NULL.
    if( argsPtr == NULL ){
      return 0u;
    }

    unsigned int argumentsCount = 1u;
    
    while (*argsPtr != 0x00){
      
      if (*argsPtr == TBCH_STRING_DELIMITER){
        argsPtr++;
        while( *argsPtr != TBCH_STRING_DELIMITER && (*argsPtr != 0x00) ){
          argsPtr++;
        }
      };

      if (*argsPtr == TBCH_ARGUMENT_SEPARATOR) argumentsCount++;
      if (*argsPtr == 0x00) continue;
      argsPtr++;
    };

    return argumentsCount;
  }

  char* getArgumentNumberPtr(char* buffer, unsigned int argumentNumber){

    char* command = TBCommandsParser::goToAfterInitiator(buffer);

    // Check if there is such a number of arguments.
    if( argumentNumber > TBCommandsParser::countArguments(command)){
      return NULL;
    }

    // Find the "=" sign and go to the first character, which is where the first argument starts.
    char* argsPtr = strchr(command,'=');

    // If there is no equal sign, then we don't have any arguments.
    if(argsPtr == NULL){
      return NULL;
    }

    // The next position is the first character of the first argument.
    argsPtr++;
    
    // If we've reached the end of the string, then there are no arguments with values.
    if(*argsPtr == 0x00){
      return NULL;
    }

    // If the first argument is the one required, then we've found it
    // and the pointer is in position. No need to run the loop.
    if(argumentNumber == 1u){

      // If the supposed arguments's first char is an argument separator,
      // then the arguments value is NULL.
      if(*argsPtr == TBCH_ARGUMENT_SEPARATOR){
        return NULL;
      }

      return argsPtr;
    }

    // If we need another argument, then we are at argument number 1.
    unsigned int argumentsCount = 1u;
    
    // We need to continue searching for a separator from the next char.
    argsPtr++;
    
    // Checking each character while in the char array.
    while (*argsPtr != 0x00) {
      
      // If a TBCH_ARGUMENT_SEPARATOR is found, then we've got a new argument.
      if (*argsPtr == TBCH_ARGUMENT_SEPARATOR){
        argumentsCount++;
        
        // Go to the argument's first character.
        argsPtr++;

        if (*argsPtr == TBCH_ARGUMENT_SEPARATOR){
          continue;
        }
        
        // If the this is null character after the TBCH_ARGUMENT_SEPARATOR,
        // then the argument's value is NULL.
        if (*argsPtr == 0x00){
          return NULL;
        }

        // If we are at the correct argumentNumber, then we've found it.
        if(argumentsCount == argumentNumber){

          if(*argsPtr == TBCH_ARGUMENT_SEPARATOR){
            return NULL;
          }

          return argsPtr;
        }

        // If the current argument is a delimited string, then we need to go through it.
        if (*argsPtr == TBCH_STRING_DELIMITER){
          argsPtr++;
          while( *argsPtr != TBCH_STRING_DELIMITER && *argsPtr != 0x00 ){
            argsPtr++;
          }
          if (*argsPtr == 0x00) return NULL;
        };

      }
      
      // Go to the next character to continue searching for the next TBCH_ARGUMENT_SEPARATOR.
      argsPtr++;
    };

    // Argument number doesn't exist. Unnecesary because we've checked at the beginning
    // but it's safer to keep this here, for now.
    return NULL;
  }

  unsigned int getArgumentLength(char* bufferArgPtr){

    if( TBCommandsParser::canBeNullArgType(bufferArgPtr) ) return 0u;

    char* argsPtr = bufferArgPtr;
    unsigned int charCount = 0;

    if(*argsPtr == '"'){

      argsPtr++;

      // Parse until string delimiter found.
      while( *argsPtr != TBCH_STRING_DELIMITER ){

        // It the character is not a printable one or the end of the string
        // has been reached without finding the end-side delimiter, 
        // then this isn't a valid string.
        if( *argsPtr == 0x00 || !isprint( *argsPtr ) ){
          return charCount;
        }
        charCount++;
        argsPtr++;
      }
      return charCount;
    }
    
    // Parse until non-char or argument separator found.
    while( *argsPtr != TBCH_ARGUMENT_SEPARATOR && *argsPtr != 0x00 ){

      // It the character is not a printable one, then this isn't a valid string.
      if( !isprint( *argsPtr ) ){
        return charCount;
      }
      charCount++;
      argsPtr++;
    }

    return charCount;
  }

  int getArgumentLength(char* buffer, unsigned int argumentNumber){
    char* bufferArgPtr = TBCommandsParser::getArgumentNumberPtr(buffer, argumentNumber);
    return TBCommandsParser::getArgumentLength(bufferArgPtr);
  }

  bool canBeNullArgType(char* bufferArgPtr){
    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return true;
    return false;
  }

  bool canBeCharArgType(char* bufferArgPtr){

    if( TBCommandsParser::canBeNullArgType(bufferArgPtr) ) return false;

    
    if( *(bufferArgPtr+1) != TBCH_ARGUMENT_SEPARATOR && (bufferArgPtr+1) != NULL ) return false;

    return true;
  }

  bool canBeCharArrayArgType(char* bufferArgPtr){
    
    if( TBCommandsParser::canBeNullArgType(bufferArgPtr) ) return false;

    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return false;

    char* argsPtr = bufferArgPtr;

    if(*argsPtr == '"'){

      argsPtr++;

      // Parse until string delimiter found.
      while( *argsPtr != TBCH_STRING_DELIMITER ){

        // It the character is not a printable one or the end of the string
        // has been reached without finding the end-side delimiter, 
        // then this isn't a valid string.
        if( *argsPtr == 0x00 || !isprint( *argsPtr ) ){
          return false;
        }
        argsPtr++;
      }
      return true;
    }
    
    // Parse until non-char or argument separator found.
    while( *argsPtr != TBCH_ARGUMENT_SEPARATOR && *argsPtr != 0x00 ){

      // It the character is not a printable one, then this isn't a valid string.
      if( !isprint( *argsPtr ) ) return false;
      argsPtr++;
    }

    return true;
  }

  bool canBeLongArgType(char* bufferArgPtr){

    if( TBCommandsParser::canBeNullArgType(bufferArgPtr) ) return false;

    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return false;

    char* argsPtr = bufferArgPtr;

    if( *argsPtr == '-' ){
      argsPtr++;
      if(*argsPtr == TBCH_ARGUMENT_SEPARATOR || *argsPtr == 0x00){
        return false;
      }
    }

    while(*argsPtr != TBCH_ARGUMENT_SEPARATOR && *argsPtr != 0x00){
      if( !isdigit(*argsPtr) ) return false;
      argsPtr++;
    }

    return true;
  }

  bool canBeUnsignedLongArgType(char* bufferArgPtr){

    if( TBCommandsParser::canBeNullArgType(bufferArgPtr) ) return false;

    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return false;

    char* argsPtr = bufferArgPtr;

    while(*argsPtr != TBCH_ARGUMENT_SEPARATOR && *argsPtr != 0x00){
      if( !isdigit(*argsPtr) ) return false;
      argsPtr++;
    }

    return true;
  }

  bool canBeFloatArgType(char* bufferArgPtr){

    if( TBCommandsParser::canBeNullArgType(bufferArgPtr) ) return false;

    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return false;

    char* argsPtr = bufferArgPtr;
    bool pointFound = false;

    if( *argsPtr == '-' ){
      argsPtr++;
      if(*argsPtr == TBCH_ARGUMENT_SEPARATOR || *argsPtr == 0x00){
        return false;
      }
    }

    if( *argsPtr == '.' ){
      pointFound = true;
      argsPtr++;

      if(*argsPtr == TBCH_ARGUMENT_SEPARATOR || *argsPtr == 0x00 || !isdigit(*argsPtr)){
        return false;
      }
    }

    while(*argsPtr != TBCH_ARGUMENT_SEPARATOR && *argsPtr != 0x00){
      
      if( *argsPtr == '.' ){
        if( !pointFound ) {
          pointFound = true;
          argsPtr++;
          continue;
        } else {
          return false;
        }
      }

      if( !isdigit(*argsPtr) ) return false;
      argsPtr++;
    }

    if( !pointFound ) return false;

    return true;
  }

  int inferArgumentType(char* bufferArgPtr){

    // []
    if( TBCommandsParser::canBeNullArgType(bufferArgPtr) ){
      return TBCommandsParser::TBArgumentType::nullArg;
    }
    
    // Si tiene al menos un caracter.
    // [.]*
    if( TBCommandsParser::canBeCharArrayArgType(bufferArgPtr) ){

      // Si tiene solamente un caracter.
      // [.]
      if( TBCommandsParser::canBeCharArgType(bufferArgPtr) ){

        // Si el caracter es un número.
        // [\d]
        if( TBCommandsParser::canBeUnsignedLongArgType(bufferArgPtr) ){
          return TBCommandsParser::TBArgumentType::unsignedLongArg;
        }

        return TBCommandsParser::TBArgumentType::charArg;
      }

      // Si tiene más de un caracter.

      // Si es un float.
      // [\-,\d,\.]
      if( TBCommandsParser::canBeFloatArgType(bufferArgPtr) ){
        return TBCommandsParser::TBArgumentType::floatArg;
      } 

      // Si es número sin punto.
      // [\-,\d]
      if( TBCommandsParser::canBeLongArgType(bufferArgPtr) ){

        // Si no tiene signo negativo.
        // [\d]
        if( TBCommandsParser::canBeUnsignedLongArgType(bufferArgPtr) ){
          return TBCommandsParser::TBArgumentType::unsignedLongArg;
        }

        return TBCommandsParser::TBArgumentType::longArg;
      };

      return TBCommandsParser::TBArgumentType::charArrayArg;
    }

    return TBCommandsParser::TBArgumentType::nullArg;
  }

  int inferArgumentType(char* buffer, unsigned int argumentNumber){
    char* bufferArgPtr = TBCommandsParser::getArgumentNumberPtr(buffer, argumentNumber);
    return TBCommandsParser::inferArgumentType(bufferArgPtr);
  }

  bool parseCharArg(char* bufferArgPtr, char* charArgPtr){
    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return false;

    if( *(bufferArgPtr+1) != TBCH_ARGUMENT_SEPARATOR && (bufferArgPtr+1) != NULL ) return false;

    *charArgPtr = *bufferArgPtr;

    return true;
  }

  bool parseCharArrayArg(char* bufferArgPtr, char* charArrayArgPtr){

    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return false;

    char* argsPtr = bufferArgPtr;

    if(*argsPtr == '"'){

      argsPtr++;

      // Parse until string delimiter found.
      while( *argsPtr != TBCH_STRING_DELIMITER ){

        // It the character is not a printable one or the end of the string
        // has been reached without finding the end-side delimiter, 
        // then this isn't a valid string.
        if( *argsPtr == 0x00 || !isprint( *argsPtr ) ){
          return false;
        }
        *charArrayArgPtr = *argsPtr;
        charArrayArgPtr++;
        argsPtr++;
      }
      return true;
    }
    
    // Parse until non-char or argument separator found.
    while( *argsPtr != TBCH_ARGUMENT_SEPARATOR && *argsPtr != 0x00 ){

      // If the character is not a printable one, then this isn't a valid string.
      if( !isprint( *argsPtr ) ){
        return false;
      }

      *charArrayArgPtr = *argsPtr;
      charArrayArgPtr++;
      argsPtr++;
    }

    return true;
  }
  
  bool parseLongArg(char* bufferArgPtr, long* longArgPtr){

    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return false;

    bool isNegative = false;

    if( *bufferArgPtr == '-' ){
      isNegative = true;
      bufferArgPtr++;  // Move pointer to first digit.
    }

    if( !isdigit(*bufferArgPtr) ) return false;
    *longArgPtr = 0;
    while( isdigit(*bufferArgPtr) ){
      *longArgPtr = *longArgPtr * 10 + *bufferArgPtr - '0';
      bufferArgPtr++;  // Move pointer one char forward.
    };

    if(isNegative){
      *longArgPtr = -*longArgPtr;
    }
    
    return true;
  }

  bool parseUnsignedLongArg(char* bufferArgPtr, unsigned long* unsignedLongArgPtr){

    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return false;

    // The number shouldn't be negative.
    if( *bufferArgPtr == '-' ){
      return false;
    }
    if( !isdigit(*bufferArgPtr) ) return false;
    *unsignedLongArgPtr = 0;
    while( isdigit(*bufferArgPtr) ){
      *unsignedLongArgPtr = *unsignedLongArgPtr * 10 + *bufferArgPtr - '0';
      bufferArgPtr++;  // Move pointer one char forward.
    };
    
    return true;
  }

  bool parseFloatArg(char* bufferArgPtr, float* floatArgPtr){

    if(bufferArgPtr == NULL || *bufferArgPtr == TBCH_ARGUMENT_SEPARATOR) return false;

    // The number shouldn't be negative.
    if( *bufferArgPtr == '-' ){
      return false;
    }
    if( !isdigit(*bufferArgPtr) ) return false;
    *floatArgPtr = 0;
    while( isdigit(*bufferArgPtr) ){
      *floatArgPtr = *floatArgPtr * 10 + *bufferArgPtr - '0';
      bufferArgPtr++;  // Move pointer one char forward.
    };
    
    return true;
  }
}
