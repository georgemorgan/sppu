#include "cpu.h"

#include "M6502/M6502.h"

/* ~ Create an instance of the CPU. ~ */

static M6502 cpu;

/* ~ Called periodically during execution to return program flow to the supervisor. ~ */

byte Loop6502(M6502 *R) {
	return INT_NONE;
}

/* ~ Called when the CPU requests the next opcode. ~ */

byte Op6502(word Addr) {
	return Rd6502(Addr);
}

/* ~ Called when an unknown opcode is executed. ~ */

byte Patch6502(byte Op, M6502 *R) {
	return 0;
}

/* ~ Called when the CPU reads from a memory address. ~ */

byte Rd6502(word Addr) {
	return nes_cpu_read(Addr);
}

/* ~ Called when the CPU writes to a memory address. ~ */

void Wr6502(word Addr, byte Value) {
	nes_cpu_write(Addr, Value);
}

void cpu_begin(void) {
	/* ~ Reset the CPU. ~ */
	Reset6502(&cpu);

	/* ~ Begin CPU emulation. ~ */
	while (1) {
		for (int scanline = 0; scanline < 262; scanline ++) {
			/* ~ Execute the number of instructions the CPU performs during a scanline. ~ */
			Exec6502(&cpu, 144);

			/* ~ Simulate an NMI during the VBLANK period. ~ */
			if (scanline == 239) Int6502(&cpu, INT_NMI);
		}
	}
}

#ifdef DESKTOP

/* ~ Initializes the CPU and begins emulation. ~ */

int main(int argc, char *argv[]) {

	FILE *romfile = fopen(argv[1], "r");

	/* ~ Ensure that the ROM was opened successfully. ~ */

	if (!romfile) { printf("Could not open ROM file '%s'.\n", argv[1]); return 0; }

	/* ~ Obtain the size of the file. ~ */

	fseek(romfile, 0L, SEEK_END);

	size_t size = ftell(romfile);

	/* ~ Skip the INES header. ~ */

	fseek(romfile, 0x10, SEEK_SET);

	/* ~ Read the ROM into the buffer. ~ */

	fread(rom, 0x8000, 1, romfile);

	/* ~ Close the ROM. ~ */

	fclose(romfile);

	return 0;

}

#endif
