#ifndef CPU_STACK_H
#define CPU_STACK_H

/**********************
 *      Stack .H      *
 **********************/
 
#include "StackExceptions.h"

#include <cstdint>

// 64000 Bytes (64kB)
#define CPU8086_STACK_SIZE 0xFFFF
 
 	/**
	 * @author Cameron Sims
	 * @date 28/05/2024
	 * @version 1.0
	 *
	 * @class Stack
	 * @brief A Stack Data Structure
	 */
class Stack {
  public:
	  /**
	   * @brief Default Constructor
	   * @param p_size The size of the stack
	   */
	Stack(uint16_t p_size = CPU8086_STACK_SIZE);

		/**
		 * @brief Copy Constructor, Deep Copies
		 * @param p_stack The original stack
		 */
	Stack(const Stack& p_stack);

		/**
		 * @brief Destructor, Delete Allocated memory
		 */
	~Stack();

		/**
		 * @brief Assignment Operator, creates a copy
		 */
	Stack& operator= (const Stack& p_stack);

		/**
		 * @brief Push into the stack
		 * @param p_value Add value into the stack
		 */
	void push(uint16_t p_value);

		/**
		 * @brief Pops top
		 * @return Returns the top of the stack
		 */
	uint16_t pop();

	friend struct CPU8086;

  private:
    /// The data were using
	uint16_t* m_data;

	/// The stack pointer
	uint16_t* m_ptr;

	/// The stack size
	const uint16_t m_size;
};

#endif