#ifndef REGISTER_REGISTER_H
#define REGISTER_REGISTER_H

/**********************
 *     REGISTER.H     *
 **********************/

#include <cstdint>

template<typename T>
struct Register {
	T value;
};

    /**
     * @author Cameron Sims
     * @date 28/05/2024
     * @version 1.0
     *
     * @struct Register8
     * @brief A register, can be used to transfer values
     */
struct Register8 : public Register<uint8_t> {};

	/**
	 * @author Cameron Sims
	 * @date 28/05/2024
	 * @version 1.0
	 *
	 * @struct Register16
	 * @brief A register, can be used to transfer values
	 */
struct Register16 : public Register<uint16_t> {
	/// The lower
	uint8_t* const low  = reinterpret_cast<uint8_t*>(&value);

	/// The upper
	uint8_t* const high = low + 1;
};


#endif