#ifndef CPU_CPU_H
#define CPU_CPU_H

/**********************
 *       CPU .H       *
 **********************/
 
#include "./Stack.h"

#include "../register/DataSegmentRegister.h"
#include "../register/FlagsRegister.h"
#include "../register/Register.h"
 
 	/**
	 * @author Cameron Sims
	 * @date 28/05/2024
	 * @version 1.0
	 *
	 * @struct CPU8086
	 * @brief A 8086 Processor CPU
	 */
struct CPU8086 {
	// The Flags /////////////////////////////////////////////
	FlagsRegister FR{};

	// The Registers /////////////////////////////////////////
	

	// General Registers /////////////////////////////////////

	/// Accumulator Register
	Register16 AX;

	/// Base Register
	Register16 BX;

	/// Counter Register
	Register16 CX;

	/// Display Register
	Register16 DX;

	/////////////////////////////////////////////////////////
	
	/// Special Registers ///////////////////////////////////

	/// Stack Pointer
	Register16* SP = (Register16*)(&this->stack.m_ptr);

	/// Data Pointer
	Register16* DP;

	/// Source Index Register
	Register16 SI;

	/// Destination Register
	Register16 DI;
	
	/// Base Pointer
	Register16 BP;

	/// Instruction Pointer
	Register16 IP;

	/// State Pointer / State Register
	Register16 SR;

	// Segment Registers //////////////////////////////////

	/// Code Segment Register
	Register16 CS;

	/// Data Segment Register
	Register16 DS;
	uint8_t* data;

	/// Stack Segment Register
	Register16 SS;
	Stack stack;

	/// Extra Segment Register
	Register16 ES;


	enum class Instruction {

		NONE = 0,

		// Math Instructions ///

		/// Addition
		ADD,

		/// Subtraction
		SUB,

		/// Multiply
		MUL,

		/// Divide
		DIV,

		/// Increment
		INC,

		/// System Interupt
		INT,

		/// Decrement
		DEC,

		/// Move / Assignment 
		MOV,

		// Stack Instructions //

		/// Pop the Top of the Stack
		POP,

		/// Push to the Top of the Stack
		PUSH,

		/// Push All General Purpose Registers' values to the top of the stack
		PUSHA,

		/// Pop the Top of the Stack into General Purpose Registers
		POPA,

		PPUSH
		////////////////////////
	};
};

	/**
	 * @author Cameron Sims
	 * @date 28/05/2024
	 * @version 1.0
	 *
	 * @fn CPU8086_init
	 * @brief Initalises the CPU we're using
	 * 
	 * @param p_cpu Takes a Pointer to the CPU instance.
	 */
int CPU8086_init(CPU8086* const p_cpu);

    /**
     * @author Cameron Sims
     * @date 28/05/2024
     * @version 1.0
     *
     * @fn CPU8086_close
     * @brief Frees all memory associated
     *
     * @param p_cpu Takes a Pointer to the CPU instance.
     */
int CPU8086_close(CPU8086* const p_cpu);

    /**
     * @author Cameron Sims
     * @date 29/05/2024
     * @version 1.0
     *
     * @fn CPU8086_resetFlags
     * @brief Sets the flags register to 0
     *
     * @param p_cpu Takes a Pointer to the CPU instance.
     *
     * @returns True if successful
     */
int CPU8086_resetFlags(CPU8086* const p_cpu);


    /**
     * @author Cameron Sims
     * @date 29/05/2024
     * @version 1.0
     *
     * @fn CPU8086_examineFlags
     * @brief Sets the flags register for bits
     *
     * @param p_cpu Takes a Pointer to the CPU instance.
     * @param p_ins The instruction that was will/be done
     * @param p_old The old value for an register
     * @param p_new A new value for a register
     * 
     * @returns True if successful
     */
int CPU8086_examineFlags(CPU8086* const p_cpu, CPU8086::Instruction p_ins, uint16_t p_old, uint16_t p_new);

int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register16* const p_lParam,     Register16* const p_rParam);
int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register16* const p_lParam, const uint16_t* const p_rParam);
int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register16* const p_lParam);

int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register8* const p_lParam,     Register8* const p_rParam);
int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register8* const p_lParam, const uint8_t* const p_rParam);
int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register8* const p_lParam);

int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins);


#endif