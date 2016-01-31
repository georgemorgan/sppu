#ifndef sppu_driver_h

#define sppu_driver_h

#include <flipper/types.h>

#include <error/error.h>

#include <fmr/fmr.h>

/* ~ PPU internal addresses. ~ */

#define PPUCTRL 0x00

#define PPUMASK 0x01

#define PPUSTATUS 0x02

#define OAMADDR 0x03

#define OAMDATA 0x04

#define PPUSCROLL 0x02

#define PPUADDR 0x06

#define PPUDATA 0x07

/* ~ Declare the virtual interface for the PPU driver. ~ */

extern const struct _ppu {
	
	void (* configure)(void);
	
	void (* load)(uint32_t source, uint16_t length);
	
	void (* write)(uint32_t address, uint8_t value);
	
	uint8_t (* read)(uint32_t address);

	void (* dma)(void *source);
	
} ppu;

/* ~ Define the private definitions of the driver functions. ~ */

#ifdef __private_include__

/* ~ Declare the enum overlay for this driver. ~ */

enum { _ppu_configure, _ppu_load, _ppu_write, _ppu_read, _ppu_dma };

/* ~ Configures the necessary IO to interface with the PPU. ~ */

extern void ppu_configure(void);

/* ~ Loads an entire PPU dump into PPU memory. ~ */

extern void ppu_load(uint32_t source, uint16_t length);

/* ~ Writes to an internal PPU address. ~ */

extern void ppu_write(uint32_t address, uint8_t value);

/* ~ Reads from an internal PPU address. ~ */

extern uint8_t ppu_read(uint32_t address);

/* ~ Simulates the DMA of SPR-RAM. The length is assumed. ~ */

extern void ppu_dma(void *source);

#endif

#endif