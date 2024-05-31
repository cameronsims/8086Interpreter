#include "../../../include/interpreter/commandHandler.h"

#include <sstream>

std::map<std::string, CPU8086::Instruction> command::commandMap;
std::map<std::string, Register16*>          command::register16Map;
std::map<std::string, Register8*>           command::register8Map;

void command::createCMDMap() {
    command::commandMap["ADD"  ] = CPU8086::Instruction::ADD  ;
    command::commandMap["DEC"  ] = CPU8086::Instruction::DEC  ;
    command::commandMap["DIV"  ] = CPU8086::Instruction::DIV  ;
    command::commandMap["INC"  ] = CPU8086::Instruction::INC  ;
    command::commandMap["INT"  ] = CPU8086::Instruction::INT  ;
    command::commandMap["MOV"  ] = CPU8086::Instruction::MOV  ;
    command::commandMap["MUL"  ] = CPU8086::Instruction::MUL  ;
    command::commandMap["POP"  ] = CPU8086::Instruction::POP  ;
    command::commandMap["POPA" ] = CPU8086::Instruction::POPA ;
    command::commandMap["PPUSH"] = CPU8086::Instruction::PPUSH;
    command::commandMap["PUSH" ] = CPU8086::Instruction::PUSH ;
    command::commandMap["PUSHA"] = CPU8086::Instruction::PUSHA;
    command::commandMap["SUB"  ] = CPU8086::Instruction::SUB  ;
}

void command::assignRegisterMap(CPU8086* const p_cpu) {
    // Assign 16-bits with assingable 8-bit registers
    command::register16Map["AX"] = &(p_cpu->AX);
    command::register8Map ["AH"] = (Register8*)p_cpu->AX.high;
    command::register8Map ["AL"] = (Register8*)p_cpu->AX.low;

    command::register16Map["BX"] = &(p_cpu->BX);
    command::register8Map ["BH"] = (Register8*)p_cpu->BX.high;
    command::register8Map ["BL"] = (Register8*)p_cpu->BX.low;

    command::register16Map["CX"] = &(p_cpu->CX);
    command::register8Map ["CH"] = (Register8*)p_cpu->CX.high;
    command::register8Map ["CL"] = (Register8*)p_cpu->CX.low;

    command::register16Map["DX"] = &(p_cpu->DX);                      
    command::register8Map ["DH"] = (Register8*)p_cpu->DX.high;
    command::register8Map ["DL"] = (Register8*)p_cpu->DX.low;

    // Assign uncool lame registers
    command::register16Map["SI"] = &(p_cpu->SI);
    command::register16Map["DI"] = &(p_cpu->DI);
    command::register16Map["BP"] = &(p_cpu->BP);
    command::register16Map["SP"] =   p_cpu->SP ;

    command::register16Map["IP"] = &(p_cpu->IP);
    command::register16Map["SR"] = &(p_cpu->SR);

    command::register16Map["CS"] = &(p_cpu->CS);
    command::register16Map["DS"] = &(p_cpu->DS);
    command::register16Map["SS"] = &(p_cpu->SS);
    command::register16Map["ES"] = &(p_cpu->ES);
}

command::Command<uint16_t> command::parse(const std::string& p_str) {
    // The command show be in one of the following values:
    // 
    // **********************
    // *                    *
    // * CMD                *
    // * CMD LPARAM         *
    // * CMD LPARAM, RPARAM *
    // *                    *
    // **********************
    //

    // Stringstream
    std::stringstream ss(p_str);

    // Loop while there is still stuff
    while (!ss.eof()) {
        // Get one line...
        std::string line;
        std::getline(ss, line);
        std::stringstream ls(line);

        // See if there is a comma...
        intmax_t spacePos = -1;
        intmax_t commaPos = -1;
        intmax_t end = line.size();
        for (size_t i = 0; i < line.size(); i++) {

            if (spacePos == -1 && line[i] == CPU8086_ASM_WHITESPACE) {
                spacePos = i;
            }

            if (spacePos != -1 && line[i] == CPU8086_ASM_COMMA) {
                commaPos = i;
                break;
            }

            // If there is a comma
            if (line[i] == CPU8086_ASM_COMMENT) {
                end = i;
                break;
            }
        }

        std::string cmdName = "",
                    lParam  = "",
                    rParam  = "";

        cmdName = line.substr(0, spacePos);

        // Check if invalid
        if (spacePos > -1) {
            if (commaPos > -1) {
                lParam = line.substr(spacePos + 1, commaPos - (commaPos - spacePos + 1));
                rParam = line.substr(commaPos + 1, end);
            } else {
                lParam = line.substr(spacePos + 1, end);
            }
        }

        // Get rid of annoying whitespace
        ss = std::stringstream(cmdName);
        ss >> cmdName;

        ss = std::stringstream(lParam);
        ss >> lParam;

        ss = std::stringstream(rParam);
        ss >> rParam;

        // Parse the strings using another function
        return command::parse(cmdName, lParam, rParam);
    }

    command::Command<uint16_t> cmd;
    return cmd;
}

command::Command<uint16_t> command::parse(const std::string& p_ins, const std::string& p_lParam, const std::string& p_rParam) {
    // The command in question
    command::Command<uint16_t> cmd;

    // Get the instruction... if it matches our interpreter map...

    cmd.instruction = command::commandMap[p_ins];
    
    // Check parameters for registers
    if (p_lParam.size() > 0) {

        if (command::register16Map.find(p_lParam) == command::register16Map.end()) {
            // Check 8-bit instead
            if (command::register8Map.find(p_lParam) == command::register8Map.end()) {
                // If not found throw again
                throw command::InvalidName();
            } else {
                // Set the register
                cmd.registers = (Register16*)command::register8Map[p_lParam];
            }
        } else {
            cmd.registers = command::register16Map[p_lParam];
        }
    }

    // Check parameters for variables

    // Check parameters for values
    if (p_rParam.size() > 0) {
        try {        
            uint16_t temp = 0;

            enum class DIGIT_TYPE {
                HEXADECIMAL = 0,
                DECIMAL,
                BINARY
            };

            // 0x... 0x...H ...H are all hexadecimal digits we're looking to use
            // 0b... is a binary digit
            // 0... is a hexadecimal digit
            DIGIT_TYPE type = DIGIT_TYPE::DECIMAL;

            if (p_rParam[1] == 'x' || p_rParam[ p_rParam.size() - 1] == 'H') {
                type = DIGIT_TYPE::HEXADECIMAL;
            }

            if (p_rParam[1] == 'b') {
                type = DIGIT_TYPE::BINARY;
            }

            switch (type) {
                case DIGIT_TYPE::HEXADECIMAL: {

                    temp = std::stoi(p_rParam, nullptr, 16);

                    /*bool prefix = (p_rParam[0] == '0' && p_rParam[1] == 'x');
                bool suffix = (p_rParam[p_rParam.size() - 1]);
                
                temp = 0;

                const size_t start = (prefix) ? 2 : 0;
                const size_t end = (suffix) ? p_rParam.size() - 1 :p_rParam.size();

                for (size_t i = end - 1; i >= start; i--) {
                    const char& c = p_rParam[i];
                    uint8_t digit = 0;

                    if (c >= 0 && c <= '9') {
                        digit = c - '9';
                    } else if (c >= 'A' && c <= 'F') {
                        digit = (c - 'A') + 10;
                    }

                    else if (c >= 'a' && c <= 'f') {
                        digit = (c - 'a') + 10;
                    } else {
                        std::stoi("CAUSE AN ERROR MUWAHAHAAH!");
                    }

                    temp += 16 * i * (digit);
                }
                */
                    break;
                }
                case DIGIT_TYPE::DECIMAL: {
                    temp = std::stoi(p_rParam, nullptr, 10);
                    break;
                }
                case DIGIT_TYPE::BINARY: {
                    std::string cut = p_rParam.substr(2, p_rParam.size());
                    temp = std::stoi(cut, nullptr, 2);

                    break;
                }
            }

            cmd.values = new uint16_t(temp);
        } catch (const std::exception&) {
            // Throw a cooler error
            throw command::InvalidNumeral();
        }
    }

    return cmd;
}

int command::execute(CPU8086* const p_cpu, const std::string& p_str) {
    // Get the command
    Command cmd = command::parse(p_str);

    // Check if there is a command
    if (cmd.instruction == CPU8086::Instruction::NONE) {
        return false;
    }
    
    return CPU8086_execute(p_cpu, cmd.instruction, (Register16* const)cmd.registers, (const uint16_t* const)cmd.values);
}