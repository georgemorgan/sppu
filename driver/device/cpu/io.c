#include "cpu.h"

/* ~ Allocate space in the .data section for RAM. ~ */

static uint8_t ram[NES_RAM_SIZE];

static const uint8_t rom[NES_ROM_SIZE];

uint8_t nes_cpu_read_ram(uint16_t address) {
	
	return ram[address % 0x800];
	
}

void nes_cpu_write_ram(uint16_t address, uint8_t value) {
	
	ram[address % 0x800] = value;
	
}

uint8_t nes_cpu_read_io(uint16_t address) {
	
	switch (address) {
			
		/* ~ PPU 0x2000 - 0x2007 ~ */
			
		case 0x2000:
			
			/* ~ PPU CTL1 ~ */
			
			break;
			
		case 0x2001:
			
			/* ~ PPU CTL2 ~ */
			
			break;
			
		case 0x2002:
			
			/* ~ PPU SREG ~ */
		
			break;
			
		case 0x2003:
			
			/* ~ PPU SPR-RAM LATCH ~ */
			
			break;
			
		case 0x2004:
			
			/* ~ PPU SPR-RAM DATA ~ */
			
			break;
			
		case 0x2005:
			
			/* ~ VRAM SCROLL ~ */
			
			break;
			
		case 0x2006:
			
			/* ~ VRAM LATCH ~ */
			
			break;
			
		case 0x2007:
			
			/* ~ VRAM DATA ~ */
			
			break;
			
		/* ~ APU 0x4000 - 0x4013 ~ */
			
		case 0x4000:
			
			break;
			
		case 0x4001:
			
			break;
			
		case 0x4002:
			
			break;
			
		case 0x4003:
			
			break;
			
		case 0x4004:
			
			break;
			
		case 0x4005:
			
			break;
			
		case 0x4006:
			
			break;
			
		case 0x4007:
			
			break;
			
		case 0x4008:
			
			break;
			
		case 0x4009:
			
			break;
			
		case 0x400a:
			
			break;
			
		case 0x400b:
			
			break;
			
		case 0x400c:
			
			break;
			
		case 0x400d:
			
			break;
			
		case 0x400e:
			
			break;
			
		case 0x400f:
			
			break;
			
		case 0x4010:
			
			break;
			
		case 0x4011:
			
			break;
			
		case 0x4012:
			
			break;
			
		case 0x4013:
			
			break;
			
		/* ~ PPU SPR-RAM DMA 0x4014 ~ */
			
		case 0x4014:
			
			/* ~ SPR_RAM DMA ~ */

			break;
			
		/* ~ APU 0x4015 ~ */
			
		case 0x4015:

			break;
			
		/* ~ Joypad 0x4016 - 0x4017 ~ */
			
		case 0x4016:
			
		/* ~ Controller A ~ */

			break;
			
		case 0x4017:
			
		/* ~ Controller B ~ */

			break;
			
		default:
			
			break;
			
	}
	
	return 0;
	
}

void nes_cpu_write_io(uint16_t address, uint8_t value) {
	
	switch (address) {
			
		/* ~ PPU 0x2000 - 0x2007 ~ */
			
		case 0x2000:
			
			/* ~ PPU CTL1 ~ */
			
			break;
			
		case 0x2001:
			
			/* ~ PPU CTL2 ~ */
			
			break;
			
		case 0x2002:
			
			/* ~ PPU SREG ~ */
			
			break;
			
		case 0x2003:
			
			/* ~ PPU SPR-RAM LATCH ~ */
			
			break;
			
		case 0x2004:
			
			/* ~ PPU SPR-RAM DATA ~ */
			
			break;
			
		case 0x2005:
			
			/* ~ VRAM SCROLL ~ */
			
			break;
			
		case 0x2006:
			
			/* ~ VRAM LATCH ~ */
			
			break;
			
		case 0x2007:
			
			/* ~ VRAM DATA ~ */
			
			break;
			
		/* ~ APU 0x4000 - 0x4013 ~ */
			
		case 0x4000:
			
			break;
			
		case 0x4001:
			
			break;
			
		case 0x4002:
			
			break;
			
		case 0x4003:
			
			break;
			
		case 0x4004:
			
			break;
			
		case 0x4005:
			
			break;
			
		case 0x4006:
			
			break;
			
		case 0x4007:
			
			break;
			
		case 0x4008:
			
			break;
			
		case 0x4009:
			
			break;
			
		case 0x400a:
			
			break;
			
		case 0x400b:
			
			break;
			
		case 0x400c:
			
			break;
			
		case 0x400d:
			
			break;
			
		case 0x400e:
			
			break;
			
		case 0x400f:
			
			break;
			
		case 0x4010:
			
			break;
			
		case 0x4011:
			
			break;
			
		case 0x4012:
			
			break;
			
		case 0x4013:
			
			break;
			
		/* ~ PPU SPR-RAM DMA 0x4014 ~ */
			
		case 0x4014:
			
			/* ~ SPR_RAM DMA ~ */
			
			break;
			
		/* ~ APU 0x4015 ~ */
			
		case 0x4015:
			
			break;
			
		/* ~ Joypad 0x4016 - 0x4017 ~ */
			
		case 0x4016:
			
			/* ~ Controller A ~ */
			
			break;
			
		case 0x4017:
			
			/* ~ Controller B ~ */
			
			break;
			
		default:
			
			break;
			
	}
	
}

uint8_t nes_cpu_read_rom(uint16_t address) {
	
	return rom[address % 0x8000];
	
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
	
	/* ~ INVALID READ ~ */
	
	else { printf("Invalid read.\n"); exit(EXIT_FAILURE); }
	
	return 0;
	
}

void nes_cpu_write(uint16_t address, uint8_t value) {
	
	/* ~ ADDRESS 0x0000 - 0x2000 (RAM, rw) ~ */
	
	if (address < 0x2000) nes_cpu_write_ram(address, value);
	
	/* ~ ADDRESS 0x2000 - 0x4020 (IO, rw) ~ */
	
	else if (address >= 0x2000 && address < 0x4020) nes_cpu_write_io(address, value);
	
	/* ~ ADDRESS 0x4020 - 0x8000 (SRAM, unimplemented) ~ */
	
	else if (address >= 0x4020 && address <= 0xFFFF) { }
	
	/* ~ ADDRESS 0x8000 - 0x10000 (ROM, rx) ~ */
	
	/* ~ INVALID WRITE ~ */
	
	else { printf("Invalid write.\n"); exit(EXIT_FAILURE); }
	
}