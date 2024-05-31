#include "../../include/interpreter/commandHandlerExceptions.h"

const char* command::InvalidToken::what() {
    return "Invalid Token Entered";
}

const char* command::InvalidName::what() {
    return "Invalid Name Entered";
}

const char* command::InvalidNumeral::what() {
    return "Invalid Number Entered";
}
