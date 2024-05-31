#ifndef REGISTER_FLAGS_H
#define REGISTER_FLAGS_H

#include <cstdint>

struct FlagsRegister {
	
	/// Overflow Flag
	uint16_t OF : 5;

	// Control Flags //////////////

	/// Direction Flag
	uint16_t DF : 1;

	/// Interrupt Flag
	uint16_t IF : 1;

	/// Trap/Trace Flag
	uint16_t TF : 1;

	///////////////////////////////

	// Condtion Flags /////////////

	/// Sign Flag
	uint16_t SF : 1;

	/// Zero Flag
	uint16_t ZF : 1;

	/// Auxiliary Carry Flag
	uint16_t AF : 2;

	/// Parity Flag
	uint16_t PF : 2;
	
	/// Carry flag
	uint16_t CF : 2;

	////////////////////////////////
};

#endif