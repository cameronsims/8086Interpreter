/**********************
 *     Stack .CPP     *
 **********************/

#include "../../../include/8086/cpu/Stack.h"

#include <memory>

Stack::Stack(uint16_t p_size) : m_size(p_size) {
	// Create data
	this->m_data = (uint16_t*)malloc( m_size * sizeof( uint16_t ) );
	this->m_ptr = this->m_data;
}
	
Stack::Stack(const Stack& p_stack) : m_size(p_stack.m_size) {
	// Free value already at pointer
	free(this->m_data);
	
	// Creates/Copies the value
	this->m_data = (uint16_t*)malloc( p_stack.m_size * sizeof(uint16_t) );
	this->m_ptr = (this->m_data) + (p_stack.m_ptr - p_stack.m_data);
	
	memcpy(m_data, p_stack.m_data, p_stack.m_size);
}
	
Stack::~Stack() {
	// Free the data
	free(this->m_data);
}
	
Stack& Stack::operator= (const Stack& p_stack) {
	// Free value already at pointer
	free(this->m_data);

	// Creates/Copies the value
	this->m_data = (uint16_t*)malloc(p_stack.m_size * sizeof(uint16_t));

	memcpy(m_data, p_stack.m_data, p_stack.m_size);

	return (*this);
}
	
void Stack::push(uint16_t p_value) {
	// If the pointer is past the end of the stack's end...
	if (m_ptr >= (m_data + sizeof(uint16_t) * m_size)) {
		throw StackOverflow();
	}

	m_data[ (m_ptr - m_data)/sizeof(uint16_t) ] = p_value;
	m_ptr = m_ptr + sizeof(uint16_t);
}
	
uint16_t Stack::pop() {
	// If the pointer is below the start
	if (m_ptr <= m_data) {
		throw StackUnderflow();
	}

	m_ptr = m_ptr - sizeof(uint16_t);
	uint16_t value = m_data[(m_ptr - m_data) / sizeof(uint16_t)];
	return value;
}