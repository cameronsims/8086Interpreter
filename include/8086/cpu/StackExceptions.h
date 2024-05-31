#ifndef CPU_STACKEXCEPTIONS_H
#define CPU_STACKEXCEPTIONS_H

/**********************
 * StackExceptions .H *
 **********************/
 
class StackError {
public:
	const char* what();
};

class StackOUflow : public StackError {
public:
	const char* what();
};

class StackOverflow : public StackOUflow{
public:
	const char* what();
};

class StackUnderflow : public StackOUflow {
public:
	const char* what();
};

#endif