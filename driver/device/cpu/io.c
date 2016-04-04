#include "cpu.h"
#define __private_include__
#include <ppu.h>
#include <flipper/at45.h>
#include <flipper/spi.h>

/* ~ Allocate space in the .data section for the NES' RAM. ~ */
static uint8_t ram[NES_RAM_SIZE];

uint8_t nes_cpu_read_ram(uint16_t address) {
	return ram[address % 0x800];
}

void nes_cpu_write_ram(uint16_t address, uint8_t value) {
	ram[address % 0x800] = value;
}

uint8_t nes_cpu_read_io(uint16_t address) {

	/* ~ PPU 0x2000 - 0x2007 ~ */
	if (address >= 0x2000 && address <= 0x2007) {
		return ppu_read_internal(address);
	}

	/* ~ Controller A - 0X4016 ~ */
	else if (address == 0x4016) {

	}

	return 0;

}

void nes_cpu_write_io(uint16_t address, uint8_t value) {

	/* ~ PPU 0x2000 - 0x2007 ~ */
	if (address >= 0x2000 && address <= 0x2007) {
		ppu_write_internal(address, value);
	}

	/* ~ OAM DMA 0x4014 ~ */
	else if (address == 0x4014) {

	}

}

/* ~ Create a variable that can be used to keep track of the previous program counter. ~ */
uint16_t previous_pc;

uint8_t nes_cpu_read_rom(uint16_t address) {

	/* ~ If the current address isn't the last address incremented, then a branch was performed. ~ */
	if (address != ++ previous_pc) {

		/* ~ Begin a new continuous read. ~ */
		at45.read(_prg_rom + address);

		previous_pc = address;

	}

	return at45.get();

}

uint8_t nes_cpu_read(uint16_t address) {

	/* ~ ADDRESS 0x0000 - 0x2000 (RAM, rw) ~ */
	if (address < 0x2000) return nes_cpu_read_ram(address);

	/* ~ ADDRESS 0x2000 - 0x4020 (IO, rw) ~ */
	else if (address >= 0x2000 && address < 0x4020) return nes_cpu_read_io(address);

	/* ~ ADDRESS 0x4020 - 0x8000 (SRAM, unimplemented) ~ */
	else if (address >= 0x4020 && address < 0x8000) { }

	/* ~ ADDRESS 0x8000 - 0x10000 (ROM, rx) ~ */
	else if (address >= 0x8000 && address <= 0xFFFF) return nes_cpu_read_rom(address);

	return 0;

}

void nes_cpu_write(uint16_t address, uint8_t value) {

	/* ~ ADDRESS 0x0000 - 0x2000 (RAM, rw) ~ */
	if (address < 0x2000) nes_cpu_write_ram(address, value);

	/* ~ ADDRESS 0x2000 - 0x4020 (IO, rw) ~ */
	else if (address >= 0x2000 && address < 0x4020) nes_cpu_write_io(address, value);

	/* ~ ADDRESS 0x4020 - 0x8000 (SRAM, unimplemented) ~ */
	else if (address >= 0x4020 && address <= 0xFFFF) { }

}
