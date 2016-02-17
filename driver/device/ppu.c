#define __private_include__
#include <ppu.h>
#include "atmel.h"
#include <at45/at45.h>
#include <fs/fs.h>
#include <spi/spi.h>
#include <cpu/cpu.h>

/*
 *
 * PPU -> FLIPPER PINOUT
 *
 * -----------------------------------------------+
 *												  |
 *  IO PIN	  PA PIN	PPU PIN					  |
 *								 _				  |
 *	13			30			AD7   |				  |
 *	12			29			AD6   |				  |
 *	11			28			AD5   |				  |
 *	10			27			AD4   | DATA BUS	  |
 *	04			26			AD3   |				  |
 *	03			25			AD2   |				  |
 *	02			24			AD1   |				  |
 *	01			15			AD0  _|				  |
 *								 _				  |
 *	A1			20			A2	  |				  |
 *	A2			19			A1	  | ADDRESS BUS	  |
 *	A3			18			A0	 _|				  |
 *								 _				  |
 *	14			2			RW	  |	DIRECTION	  |
 *	15			1			CS	 _| LATCH		  |
 *												  |
 * -----------------------------------------------+
 *
 * RW writes on low, reads on high.
 *
 * CS is active low.
 *
 * PA19 (A2) May be used to generate a Fast Interrupt Request for servicing by the CPU.
 *    -> This will be implemented to draw contiguous frame buffers to the screen during V-BLANK.
 *
 */

#define DATAMASK ((0xFE << 23) | (1 << 15))
#define ADDRMASK (7 << 18)
#define RWMASK (1 << 2)
#define CSMASK (1 << 1)

#define DATAIN() AT91C_BASE_PIOA -> PIO_OER &= ~DATAMASK; AT91C_BASE_PIOA -> PIO_ODR |= DATAMASK;
#define DATAOUT() AT91C_BASE_PIOA -> PIO_ODR &= ~DATAMASK; AT91C_BASE_PIOA -> PIO_OER |= DATAMASK;

#define READ() AT91C_BASE_PIOA -> PIO_CODR &= ~RWMASK; AT91C_BASE_PIOA -> PIO_SODR |= RWMASK;
#define WRITE() AT91C_BASE_PIOA -> PIO_SODR &= ~RWMASK; AT91C_BASE_PIOA -> PIO_CODR |= RWMASK;

#define LATCH() AT91C_BASE_PIOA -> PIO_SODR &= ~CSMASK; AT91C_BASE_PIOA -> PIO_CODR |= CSMASK;
#define RELEASE() AT91C_BASE_PIOA -> PIO_CODR &= ~CSMASK; AT91C_BASE_PIOA -> PIO_SODR |= CSMASK;

void ppu_configure(void) {

	/* ~ Enable the pins of the data bus, address bus, and direction. ~ */
	AT91C_BASE_PIOA -> PIO_PER |= DATAMASK | ADDRMASK | RWMASK | CSMASK;

	/* ~ Configure the pins of the data bus, address bus, and direction as outputs. ~ */
	AT91C_BASE_PIOA -> PIO_ODR &= ~(DATAMASK | ADDRMASK | RWMASK | CSMASK);
	AT91C_BASE_PIOA -> PIO_OER |= DATAMASK | ADDRMASK | RWMASK | CSMASK;

	/* ~ Enable single access writes to each pin. ~ */
	AT91C_BASE_PIOA -> PIO_OWDR &= ~(DATAMASK | ADDRMASK);
	AT91C_BASE_PIOA -> PIO_OWER |= DATAMASK | ADDRMASK;

}

void usart_put_byte(AT91S_USART *usart, uint8_t byte) {

	/* ~ Wait until the USART is ready to transmit. ~ */
	while (!(usart -> US_CSR & AT91C_US_TXRDY));

	/* ~ Send the byte. ~ */
	usart -> US_THR = byte;

}

void usart_push(AT91S_USART *usart, void *source, size_t length) {

	while (length --) usart_put_byte(usart, *(uint8_t *)(source ++));

}

void ppu_load(uint16_t source, uint16_t length) {

	/* ~ Pause rendering to write to VRAM. ~ */
	PAUSE();

	/* ~ Reset the address latch. ~ */
	RESETLATCH();

	/* ~ Latch the address of the pattern tables, hi byte first. ~ */
	ppu_write_internal(PPUADDR, hi(0x0000));
	ppu_write_internal(PPUADDR, lo(0x0000));

	/* ~ TODO ~ */

	/* ~ Enable rendering. ~ */
	RENDER();

}

/* ~ Writes data into the desired internal register of the PPU. ~ */
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

/* ~ Writes data into VRAM at a specified address. ~ */
void ppu_write(uint16_t address, uint8_t value) {

	RESETLATCH();
	ppu_write_internal(PPUADDR, hi(address));
	ppu_write_internal(PPUADDR, lo(address));
	ppu_write_internal(PPUDATA, value);

}

/* ~ Reads in a value from a register internal to the PPU. ~ */
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

/* ~ Reads in a value from VRAM at a specified address. ~ */
uint8_t ppu_read(uint16_t address) {

	RESETLATCH();
	ppu_write_internal(PPUADDR, hi(address));
	ppu_write_internal(PPUADDR, lo(address));
	ppu_read_internal(PPUDATA);

	return ppu_read_internal(PPUDATA);

}

/* ~ Transfers an OAM DMA block from a specified address into the PPU. ~ */
void ppu_dma(void *source) {

}

/* ~ Loads the pattern tables in from the filesystem and launches an NES emulator. ~ */
int8_t ppu_emulate(char *rom) {

	/* ~ Load the ROM from the filesystem assuming that the 'rom' parameter is the key. ~ */
	fsp _base = fs.data(rom);
	if (!data) { error.raise(_error_file_not_found); return -1; }

	/* ~ Use the base address of the ROM to calcuate the address of the PRG-ROM in the filesystem. ~ */
	/* ~ (_base + sizeof(iNES_HEADER)) ~ */
	_prg_rom = _base + 16;

	/* ~ Use the base address of the PRG-ROM to calculate the address of the pattern tables in the filesytem. ~ */
	/* ~ (_prg_rom + sizeof(PRG_ROM)) ~ */
	fsp pattern = _prg_rom + (2 * 16 * 1024);

	/* ~ Open a continuous read from flash to pull in the pattern tables. ~ */
	at45.read(pattern);

	/* ~ Latch the address of the pattern tables to begin DMA. ~ */
	RESETLATCH();
	ppu_write_internal(PPUADDR, 0x0000);
	ppu_write_internal(PPUADDR, 0x0000);

	/* ~ Transfer the pattern tables from the filesystem into the PPU. ~ */
	for (int i = 0; i < NES_PATTERN_TABLE_SIZE; i ++) {
		ppu_write_internal(PPUDATA, spi.get());
	}

	/* ~ Finish the continuous read. ~ */
	at45.disable();

}