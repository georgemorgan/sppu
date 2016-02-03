#define __private_include__

#include <ppu.h>

#include "atmel.h"

/*
 
 PPU -> FLIPPER PINOUT
 
--------------------------------------------------+
												  |
  IO PIN	  PA PIN	PPU PIN					  |
								 _				  |
	13			30			AD7   |				  |
	12			29			AD6   |				  |
	11			28			AD5   |				  |
	10			27			AD4   | DATA BUS	  |
	04			26			AD3   |				  |
	03			25			AD2   |				  |
	02			24			AD1   |				  |
	01			15			AD0  _|				  |
								 _				  |
	A1			20			A2	  |				  |
	A2			19			A1	  | ADDRESS BUS	  |
	A3			18			A0	 _|				  |
								 _				  |
	14			2			RW	  |	DIRECTION	  |
	15			1			CS	 _| LATCH		  |
												  |
--------------------------------------------------+
 
 RW writes on low, reads on high.
 
 CS is active low.
 
 PA19 (A2) May be used to generate a Fast Interrupt Request for servicing by the CPU.
	-> This will be implemented to draw contiguous frame buffers to the screen during V-BLANK.
 
*/

#define DATAMASK ((0xFE << 23) | (1 << 15))

#define DATAIN() AT91C_BASE_PIOA -> PIO_OER &= ~DATAMASK; AT91C_BASE_PIOA -> PIO_ODR |= DATAMASK;

#define DATAOUT() AT91C_BASE_PIOA -> PIO_ODR &= ~DATAMASK; AT91C_BASE_PIOA -> PIO_OER |= DATAMASK;

#define ADDRMASK (7 << 18)

#define RWMASK (1 << 2)

#define READ() AT91C_BASE_PIOA -> PIO_CODR &= ~RWMASK; AT91C_BASE_PIOA -> PIO_SODR |= RWMASK;

#define WRITE() AT91C_BASE_PIOA -> PIO_SODR &= ~RWMASK; AT91C_BASE_PIOA -> PIO_CODR |= RWMASK;

#define CSMASK (1 << 1)

#define LATCH() AT91C_BASE_PIOA -> PIO_SODR &= ~CSMASK; AT91C_BASE_PIOA -> PIO_CODR |= CSMASK;

#define RELEASE() AT91C_BASE_PIOA -> PIO_CODR &= ~CSMASK; AT91C_BASE_PIOA -> PIO_SODR |= CSMASK;


extern uint8_t attribute[];

extern uint8_t palette[];

void ppu_configure(void) {
	
	/* ~ Enable the pins of the data bus, address bus, and direction. ~ */
	
	AT91C_BASE_PIOA -> PIO_PER |= DATAMASK | ADDRMASK | RWMASK | CSMASK;
	
	/* ~ Configure the pins of the data bus, address bus, and direction as outputs. ~ */

	AT91C_BASE_PIOA -> PIO_ODR &= ~(DATAMASK | ADDRMASK | RWMASK | CSMASK);
	
	AT91C_BASE_PIOA -> PIO_OER |= DATAMASK | ADDRMASK | RWMASK | CSMASK;
	
	/* ~ Enable single access writes to each pin. ~ */

	AT91C_BASE_PIOA -> PIO_OWDR &= ~(DATAMASK | ADDRMASK);
	
	AT91C_BASE_PIOA -> PIO_OWER |= DATAMASK | ADDRMASK;
	
	/* ~ Increment VRAM address on write to PPUDATA. ~ */
	
//	ppu_write_internal(PPUCTRL, 0x00);
	
	/* ~ Show background and sprites. ~ */
	
//	ppu_write_internal(PPUMASK, 0xA);
	
}

void ppu_load(uint16_t source, uint16_t length) {

	/* ~ Pause rendering to write to VRAM. ~ */
	
	PAUSE();
	
	/* ~ Reset the address latch. ~ */
	
	RESETLATCH();
	
	/* ~ Latch the address of the attribute tables, hi byte first. ~ */
	
	ppu_write_internal(PPUADDR, hi(0x2000));
	
	ppu_write_internal(PPUADDR, lo(0x2000));
	
	for (int i = 0; i < 0x400; i ++) {
	
		/* ~ Write the data to the attribute tables. ~ */
		
		ppu_write_internal(PPUDATA, 0x2F);
		
	}
	
	/* ~ Reset the address latch. ~ */
	
//	RESETLATCH();
//	
//	/* ~ Latch the address of the palettes, hi byte first. ~ */
//	
//	ppu_write_internal(PPUADDR, hi(PALETTEADDR));
//	
//	ppu_write_internal(PPUADDR, lo(PALETTEADDR));
//	
//	for (int i = 0; i < PALETTESIZE; i ++) {
//		
//		/* ~ Write the data to the palettes. ~ */
//		
//		ppu_write_internal(PPUDATA, i);
//		
//	}
	
	/* ~ Enable rendering. ~ */
	
	RENDER();
	
}

/* ~ Sends data to the PPU's internal registers. ~ */

void ppu_write_internal(uint16_t address, uint8_t value) {
	
	/* ~ Switch the direction of the data bus to outputs. ~ */
	
	DATAOUT();
	
	/* ~ Switch the PPU into write mode. ~ */
	
	WRITE();
	
	/* ~ Write the address and data. ~ */
	
	AT91C_BASE_PIOA -> PIO_ODSR = (address << 18) | (((value << 23) | ((value & 1) << 15)) & DATAMASK);
	
	/* ~ Assert the chip select pin to latch the data. ~ */
	
	LATCH();
	
	/* ~ Deassert chip select pin to release the PPU. ~ */
	
	RELEASE();
	
}

/* ~ Sends data to VRAM. ~ */

void ppu_write(uint16_t address, uint8_t value) {
	
	RESETLATCH();
	
	/* ~ VRAM addresses are latched hi-lo. ~ */
	
	ppu_write_internal(PPUADDR, hi(address));
	
	ppu_write_internal(PPUADDR, lo(address));
	
	ppu_write_internal(PPUDATA, value);
	
}

void ppu_fill(uint16_t address, uint8_t value, uint16_t n)
{
	PAUSE();
	
	ppu_write_internal(PPUMASK, ppu_read_internal(PPUCTRL) & 0xFB);
	
	RESETLATCH();
	
	/* ~ VRAM addresses are latched hi-lo. ~ */
	
	ppu_write_internal(PPUADDR, hi(address));
	
	ppu_write_internal(PPUADDR, lo(address));
	
	while (n--)
	{
		ppu_write_internal(PPUDATA, value);
	}
	
	RENDER();
}

uint8_t ppu_read_internal(uint16_t address) {
	
	/* ~ Switch the direction of the data bus to inputs. ~ */
	
	DATAIN();
	
	/* ~ Output the address. ~ */
	
	AT91C_BASE_PIOA -> PIO_ODSR = (address << 18) & ADDRMASK;
	
	/* ~ Switch the PPU into read mode. ~ */
	
	READ();
	
	/* ~ Assert the chip select pin to latch the data. ~ */
	
	LATCH();
	
	/* ~ Read the data. We must shift based on pinout. ~ */
	
	uint8_t data = ((AT91C_BASE_PIOA -> PIO_PDSR >> 23) | ((AT91C_BASE_PIOA -> PIO_PDSR & (1 << 15)) >> 15));
	
	/* ~ Deassert chip select pin to release the PPU. ~ */
	
	RELEASE();
	
	/* ~ Return the data. ~ */
	
	return data;
	
}

uint8_t ppu_read(uint16_t address) {
	
	RESETLATCH();
	
	/* ~ VRAM addresses are latched hi-lo. ~ */
	
	ppu_write_internal(PPUADDR, hi(address));
	
	ppu_write_internal(PPUADDR, lo(address));
	
	ppu_read_internal(PPUDATA);
	
	return ppu_read_internal(PPUDATA);
	
}

void ppu_dma(void *source) {
	
	
	
}
