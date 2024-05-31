int main() {
	// Create the CPU
	CPU8086 cpu;
	CPU8086_init(&cpu);

	CPU8086::Instruction ins[4] = {
		CPU8086::Instruction::ADD,
		CPU8086::Instruction::SUB,
		CPU8086::Instruction::MUL,
		CPU8086::Instruction::DIV
	};

	for (int i = 0; i < 4; i++) {

		cpu.AX.value = 0x0009;
		cpu.BX.value = 0x0003;

		printf("0x%04x ", cpu.AX.value);
		printf("0x%04x = ", cpu.BX.value);

		if (i != 3)
			CPU8086_call<uint16_t>(&cpu, ins[i], &cpu.AX, &cpu.BX);
		else
			CPU8086_call<uint16_t>(&cpu, ins[i], &cpu.BX);

		if (i == 3) {
			printf("0x%04x r ", cpu.DX.value);
		}

		printf("0x%04x\n", cpu.AX.value);
	}

	// Kill the CPU
	CPU8086_close(&cpu);

	// Return successfully 
	return 0;
}