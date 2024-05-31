#ifndef INTERPRETER_COMMANDHANDLER_H
#define INTERPRETER_COMMANDHANDLER_H

#include "commandHandlerExceptions.h"

/**********************
 *  commandHandler.H  *
 **********************/

#define CPU8086_ASM_WHITESPACE ' '
#define CPU8086_ASM_COMMENT ';'
#define CPU8086_ASM_COMMA ','


#include "./Command.h"

#include <map>
#include <string>

namespace command {

    /// This is a map of all the commands
    extern std::map<std::string, CPU8086::Instruction> commandMap;

    /// This is a map of all the registers
    extern std::map<std::string, Register16*> register16Map;
    extern std::map<std::string, Register8*>  register8Map;



        /**
         * @author Cameron Sims
         * @date 30/05/2024
         * @version 1.0
         *
         * @brief Creates the command map
         *
         * @fn createCMDMap
         */
    void createCMDMap();

       /**
        * @author Cameron Sims
        * @date 30/05/2024
        * @version 1.0
        *
        * @brief Creates a map for the registers
        *
        * @fn assignRegisterMap
        * @param p_cpu Pointer to the CPU
        */
    void assignRegisterMap(CPU8086* const p_cpu);

        /**
         * @author Cameron Sims
         * @date 30/05/2024
         * @version 1.0
         *
         * @brief Parses a command into 1-3 segments
         * 
         * @fn parse
         * @param p_str The string we're reading from
         * @returns Returns a Command reguarding the parameters
         */
    Command<uint16_t> parse(const std::string& p_str);

        /**
         * @author Cameron Sims
         * @date 30/05/2024
         * @version 1.0
         *
         * @brief Parses 1-3 string arguments and places in the form of x86 ASM
         *
         * @fn parse
         * @param p_ins The instruction string
         * @param p_lParam The left parameter
         * @param p_rParam The right parameter
         * @returns Returns a Command reguarding the parameters
         */
    Command<uint16_t> parse(const std::string& p_ins, const std::string& p_lParam, const std::string& p_rParam);

        /**
         * @author Cameron Sims
         * @date 30/05/2024
         * @version 1.0
         *
         * @brief Runs a string
         *
         * @fn execute
         * @param p_str The line of code to run
         * @returns True if worked, False if didn't
         */
    int execute(CPU8086* const p_cpu, const std::string& p_str);
}

#endif