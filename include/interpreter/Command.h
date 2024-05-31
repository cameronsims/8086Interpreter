#ifndef INTERPRETER_COMMAND_H
#define INTERPRETER_COMMAND_H

#include "../8086/cpu/CPU.h"

namespace command {


        /**
         * @author Cameron Sims
         * @date 30/05/2024
         * @version 1.0
         *
         * @struct Command
         * @brief A command that is to be executed
         */
    template<typename T>
    struct Command {

            /**
             * @brief Default Constructor
             */
        Command();

            /**
             * @brief Destructor
             */
        ~Command();

        /// The instruction to be executed
        CPU8086::Instruction instruction;

        /// Register(s)
        Register<T>* registers = nullptr;

        /// Number(s)
        T* values = nullptr;
    };
}

template<typename T>
command::Command<T>::Command() : instruction(CPU8086::Instruction::NONE), registers(nullptr), values(nullptr) {
   
}

template<typename T>
command::Command<T>::~Command() {
    registers = nullptr;

    if (values == nullptr) {
        delete values;
        values = nullptr;
    }
}

#endif