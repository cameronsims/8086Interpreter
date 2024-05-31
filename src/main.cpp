/**********************
 *      MAIN.CPP      *
 **********************/

#include "../include/8086/cpu/CPU.h"
#include "../include/interpreter/commandHandler.h"

#include <stdio.h>

	/**
	 * @author Cameron Sims
	 * @version 1.0
	 * @date 28/05/2024
	 *
	 * @param argc The amount of files used in the program
	 * @param argv The files loaded into the program
	 * 
	 **/
int main() {
	// Create the CPU
	CPU8086 cpu;
	CPU8086_init(&cpu);

	// Create the command handler
	command::createCMDMap();
	command::assignRegisterMap(&cpu);

	command::execute(&cpu, "MOV AX, 10");
	command::execute(&cpu, "INC AX");
	
	command::execute(&cpu, "MOV BX, 0x10");
	command::execute(&cpu, "MOV CX, 0b10");


	command::execute(&cpu, "MOV DH, 255");

	printf("O--------------------------O\n");
	printf("|   R   |   RX   | RH | RL |\n");
	printf("|-------+--------+----+----|\n");
	printf("|   A   |  %04X  | %02X | %02X |\n", cpu.AX.value, *(cpu.AX.high), *(cpu.AX.low));
	printf("|   B   |  %04X  | %02X | %02X |\n", cpu.BX.value, *(cpu.BX.high), *(cpu.BX.low));
	printf("|   C   |  %04X  | %02X | %02X |\n", cpu.CX.value, *(cpu.CX.high), *(cpu.CX.low));
	printf("|   D   |  %04X  | %02X | %02X |\n", cpu.DX.value, *(cpu.DX.high), *(cpu.DX.low));
	printf("+--------------------------+\n");
	printf("|                          |\n");
	printf("+-----O-D-I-T-S-Z--A--P--C-+\n");
	printf("| 0000%01d %01d %01d %01d %01d %01d 0%01d 0%01d 0%01d |\n", cpu.FR.OF, cpu.FR.DF, cpu.FR.IF, cpu.FR.TF, cpu.FR.SF, cpu.FR.ZF, cpu.FR.AF, cpu.FR.PF, cpu.FR.CF);
	printf("O--------------------------O\n");

	// Kill the CPU
	CPU8086_close(&cpu);

	// Return successfully 
	return 0;
}