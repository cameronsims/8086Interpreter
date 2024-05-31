/**********************
 *      CPU .CPP      *
 **********************/
 
#include "../../cpu/CPU.h"

#include <memory>

int CPU8086_init(CPU8086* const p_cpu) {
	// If it failed
	if (p_cpu == NULL) {
		return false;
	}

	CPU8086_resetFlags(p_cpu);
	p_cpu->stack = Stack(CPU8086_STACK_SIZE);
	p_cpu->data = (uint8_t*)malloc(CPU8086_STACK_SIZE*sizeof(uint8_t));

	// If it failed
	if (p_cpu->data == NULL) {
		return false;
	}

	// Set register values
	p_cpu->AX.value = 0x0000;
	p_cpu->BX.value = 0x0000;
	p_cpu->CX.value = 0x0000;
	p_cpu->DX.value = 0x0000;

	// Return success
	return true;
}

int CPU8086_close(CPU8086* const p_cpu) {
	// Free the memory
	free(p_cpu->data);

	// Return success
	return true;
}


int CPU8086_resetFlags(CPU8086* const p_cpu) {
	// Reset to zero
	uint16_t empty{};
	memcpy(&(p_cpu->FR), &empty, sizeof(FlagsRegister));
	return true;
}

int CPU8086_examineFlags(CPU8086* const p_cpu, CPU8086::Instruction p_ins, uint16_t p_old, uint16_t p_new) {
	// Clear incase
	CPU8086_resetFlags(p_cpu);
		
	// For FR which need a condition alone

	// Overflow (underflow into negative)
	
	// If our old value was the same
	if ((uint16_t)p_old == (int16_t)p_old) {
		// If we're dealing with different MSBits
		if ((uint16_t)p_new != (int16_t)p_new) {
			// And the old value 
			p_cpu->FR.OF = 0b1;
		}
	}

	// If first value is signed
	if ((p_new >> ((sizeof(uint16_t) * 8) - 1)) & 1) {
		p_cpu->FR.SF = 0b1;
	}

	// If value is zero...
	if (p_new == 0) {
		p_cpu->FR.ZF = 0b1;
	}

	// Check if there is parity
	{
		uint16_t n{},
			     val = p_new;
		for (uint16_t i = 0; i < sizeof(uint16_t) * 8; i++) {
			// Check every bit
			n = n + (val & 1);
			val = val >> 1;
		}
		
		// If there is parity
		if (n % 2 == 0) {
			p_cpu->FR.PF = 0b1;
		}
	}

	// Direction is used for strings

	// Interupt is called, set the IF
	if (p_ins == CPU8086::Instruction::INT) {
		p_cpu->FR.IF = 0b1;
	}

	// Auxilliary Flag (if overflows for a nibble 0x0F -> 0x10)
	{
		// Check if the value generated overflow to the next nibble
		uint8_t* oldByte = reinterpret_cast<uint8_t*>(&p_old);
		uint8_t* newByte = reinterpret_cast<uint8_t*>(&p_new);

		for (int i = 0; i < sizeof(uint16_t); i++) {
			p_cpu->FR.AF = (((*oldByte) & 0xF) + ((*newByte) & 0xF) > 0x0F);

			if (p_cpu->FR.AF) {
				break;
			}

			oldByte++;
			newByte++;
		}
	}


	// Trap Flag (used for debugging)

	// For overflow flag which depend on instruction

	switch (p_ins) {
		// If we have added the amount...
		case CPU8086::Instruction::ADD:
		case CPU8086::Instruction::INC:
		case CPU8086::Instruction::MUL:
		{
			// If we have increased the overall amount.
			// but our value is lower
			if (p_new < p_old) {
				p_cpu->FR.AF = 0b1;
				p_cpu->FR.CF = 0b1;
			}

			break;
		}

		// If the amount has decreased
		case CPU8086::Instruction::SUB:
		case CPU8086::Instruction::DEC:
		case CPU8086::Instruction::DIV:
		{
			// If we have decreased the overall amount
			// but our value is bigger
			if (p_new > p_old) {
				p_cpu->FR.AF = 0b1;
				p_cpu->FR.CF = 0b1;
			}
		}

		break;
	}

	return true;
}

// 16-bit calls ////////////

int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register16* const p_lParam, Register16* const p_rParam) {

	CPU8086_resetFlags(p_cpu);

	return CPU8086_execute(p_cpu, p_ins, p_lParam, &(p_rParam->value));
}

int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register16* const p_lParam, const uint16_t* const p_rParam) {

	CPU8086_resetFlags(p_cpu);

	// Jump to the CPU instruction
	switch (p_ins) {
		// MATH INSTRUCTIONS /////////////////////////

		case CPU8086::Instruction::ADD: {
			// Check if we have a register
			if (p_lParam == nullptr) {
				return false;
			}
			// And something to move into the register
			if (p_rParam == nullptr) {
				return false;
			}
			
			CPU8086_examineFlags(p_cpu, p_ins, p_lParam->value, p_lParam->value + (*p_rParam));
			p_lParam->value += (*p_rParam);
			

			return true;
		}

		case CPU8086::Instruction::SUB: {
			// Check if we have a register
			if (p_lParam == nullptr) {
				return false;
			}
			// And something to move into the register
			if (p_rParam == nullptr) {
				return false;
			}

			CPU8086_examineFlags(p_cpu, p_ins, p_lParam->value, p_lParam->value - (*p_rParam));
			p_lParam->value -= (*p_rParam);
			return true;
		}

		case CPU8086::Instruction::MUL: {
			// Check if we have a register
			if (p_lParam == nullptr) {
				return false;
			}
			// And something to move into the register
			if (p_rParam == nullptr) {
				return false;
			}

			CPU8086_examineFlags(p_cpu, p_ins, p_lParam->value, (p_lParam->value) * (*p_rParam));
			p_lParam->value *= (*p_rParam);
			return true;
		}

		//////////////////////////////////////////////

		// Move Instruction
		case CPU8086::Instruction::MOV: {
			// Check if we have a register
			if (p_lParam == nullptr) {
				return false;
			}
			// And something to move into the register
			if (p_rParam == nullptr) {
				return false;
			}

			// If we do set it
			p_lParam->value = *p_rParam;
			return true;
		}


		default: {
			return CPU8086_execute(p_cpu, p_ins, p_lParam);
		}
	}

	return true;
}

int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register16* const p_lParam) {

	CPU8086_resetFlags(p_cpu);

	switch (p_ins) {

		// DIVIDE
		case CPU8086::Instruction::DIV:
		{

			if (p_lParam == nullptr)
				return false;

			// If 16-bit
			// Remainder  = AX
			// Quotient = DX

			// Divide AX/(left param)
			uint16_t numerator   = p_cpu->AX.value,
				     denominator = p_lParam->value;

			p_cpu->AX.value = numerator % denominator;
			p_cpu->DX.value = numerator / denominator;

			return true;
		}

		// Increment
		case CPU8086::Instruction::INC: {
			CPU8086_examineFlags(p_cpu, p_ins, p_lParam->value, p_lParam->value + 1);
			p_lParam->value++;
			return true;
		}

		// Decrement
		case CPU8086::Instruction::DEC: {
			CPU8086_examineFlags(p_cpu, p_ins, p_lParam->value, p_lParam->value - 1);
			p_lParam->value--;
			return true;
		}

		// Push Instruction
		case CPU8086::Instruction::PUSH:
		{
			if (p_lParam == nullptr) {
				return false;
			}

			// Push into stack
			p_cpu->stack.push(p_lParam->value);
			return true;
		}

		// Pop Instruction
		case CPU8086::Instruction::POP:
		{
			if (p_lParam == nullptr) {
				return false;
			}

			p_lParam->value = p_cpu->stack.pop();
			return true;
		}

		default:
		{
			return CPU8086_execute(p_cpu, p_ins);
		}
	}

	return true;
}

// 8-bit calls  ///////////
int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register8* const p_lParam, Register8* const p_rParam) {

	CPU8086_resetFlags(p_cpu);

	return CPU8086_execute(p_cpu, p_ins, p_lParam, &(p_rParam->value));
}

int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register8* const p_lParam, const uint8_t* const p_rParam) {
	CPU8086_resetFlags(p_cpu);

	// Jump to the CPU instruction
	switch (p_ins) {
		// MATH INSTRUCTIONS /////////////////////////

		case CPU8086::Instruction::ADD:
		{
			// Check if we have a register
			if (p_lParam == nullptr) {
				return false;
			}
			// And something to move into the register
			if (p_rParam == nullptr) {
				return false;
			}

			p_lParam->value += (*p_rParam);
			return true;
		}

		case CPU8086::Instruction::SUB:
		{
			// Check if we have a register
			if (p_lParam == nullptr) {
				return false;
			}
			// And something to move into the register
			if (p_rParam == nullptr) {
				return false;
			}

			p_lParam->value -= (*p_rParam);
			return true;
		}

		case CPU8086::Instruction::MUL:
		{
			// Check if we have a register
			if (p_lParam == nullptr) {
				return false;
			}
			// And something to move into the register
			if (p_rParam == nullptr) {
				return false;
			}

			p_lParam->value *= (*p_rParam);
			return true;
		}

		//////////////////////////////////////////////

		// Move Instruction
		case CPU8086::Instruction::MOV:
		{
			// Check if we have a register
			if (p_lParam == nullptr) {
				return false;
			}
			// And something to move into the register
			if (p_rParam == nullptr) {
				return false;
			}

			// If we do set it
			p_lParam->value = *p_rParam;
			return true;
		}


		default:
		{
			return CPU8086_execute(p_cpu, p_ins, p_lParam);
		}
	}

	return true;
}

int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins, Register8* const p_lParam) {

	CPU8086_resetFlags(p_cpu);

	switch (p_ins) {

		// DIVIDE
		case CPU8086::Instruction::DIV:
		{

			if (p_lParam == nullptr)
				return false;

			// If  8-bit
			// REG/AL = (AH/AL)
			// 
			// // Save Remainder (AH)  %
			// Save Quotient (AL)   /

			uint8_t numerator = p_lParam->value,
				denominator = (*p_cpu->AX.low);

			(*p_cpu->AX.high) = numerator % denominator;
			(*p_cpu->AX.low)  = numerator / denominator;

			return true;
		}

		// Increment
		case CPU8086::Instruction::INC: {
			p_lParam->value++;
			return true;
		}

		// Decrement
		case CPU8086::Instruction::DEC: {
			p_lParam->value--;
			return true;
		}

		default:
		{
			return CPU8086_execute(p_cpu, p_ins);
		}
	}

	return true;
}


int CPU8086_execute(CPU8086* const p_cpu, CPU8086::Instruction p_ins) {

	CPU8086_resetFlags(p_cpu);

	switch (p_ins) {

		// Push All Instruction
		case CPU8086::Instruction::PUSHA:
		{
			// AX, BX, CX, DX, SP, BP, DI, SI
			p_cpu->stack.push(p_cpu->AX.value);
			p_cpu->stack.push(p_cpu->BX.value);
			p_cpu->stack.push(p_cpu->CX.value);
			p_cpu->stack.push(p_cpu->DX.value);
			p_cpu->stack.push(p_cpu->SP->value);
			p_cpu->stack.push(p_cpu->BP.value);
			p_cpu->stack.push(p_cpu->DI.value);
			p_cpu->stack.push(p_cpu->SI.value);

			return true;
		}

		// Pop All Instruction
		case CPU8086::Instruction::POPA:
		{
			// DI, SI, BP, BX, DX, CX, and AX
			p_cpu->DI.value = p_cpu->stack.pop();
			p_cpu->SI.value = p_cpu->stack.pop();
			p_cpu->BP.value = p_cpu->stack.pop();
			p_cpu->BX.value = p_cpu->stack.pop();
			p_cpu->DX.value = p_cpu->stack.pop();
			p_cpu->CX.value = p_cpu->stack.pop();
			p_cpu->AX.value = p_cpu->stack.pop();

			return true;
		}

		default:
		{
			return false;
		}
	}

	return true;
}
