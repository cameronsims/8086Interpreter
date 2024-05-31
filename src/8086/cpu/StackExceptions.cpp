/**********************
 *StackExceptions .CPP*
 **********************/

#include "../../../include/8086/cpu/StackExceptions.h"

const char* StackError::what() {
	return "Stack Error";
}

const char* StackOUflow::what() {
	return "Stack Under/Overflow";
}

const char* StackOverflow::what() {
	return "Stack Overflow";
}
	
const char* StackUnderflow::what() {
	return "Stack Underflow";
}