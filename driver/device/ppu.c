#define __private_include__
#include <ppu.h>
#include "atmel.h"
#include <flipper/at45.h>
#include <flipper/fs.h>
#include <flipper/spi.h>
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

static const uint8_t pattern[];
static const uint8_t attribute[];
static const uint8_t palette[];

void usart_put_byte(AT91S_USART *usart, uint8_t byte);

void ppu_configure(void) {

	/* ~ Enable the RW and CS pins as well as the pins of the data and address busses. ~ */
	AT91C_BASE_PIOA -> PIO_PER |= DATAMASK | ADDRMASK | RWMASK | CSMASK;

	/* ~ Configure the RW and CS pins as well as the pins of the data and address busses. ~ */
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

	for (int i = 0; i < 0x2000; i ++) {
		
		/* ~ Write the data to the pattern table VRAM. ~ */
		ppu_write_internal(PPUDATA, pattern[i]);

	}

	/* ~ Reset the address latch. ~ */
	RESETLATCH();

	/* ~ Latch the address of the name tables, hi byte first. ~ */
	ppu_write_internal(PPUADDR, hi(0x2000));
	ppu_write_internal(PPUADDR, lo(0x2000));

	for (int i = 0; i < 0x1000; i ++) {

		/* ~ Write the data to the name table. ~ */
		ppu_write_internal(PPUDATA, attribute[i]);

	}

	/* ~ Reset the address latch. ~ */
	RESETLATCH();

	/* ~ Latch the address of the name tables, hi byte first. ~ */
	ppu_write_internal(PPUADDR, hi(0x3F00));
	ppu_write_internal(PPUADDR, lo(0x3F00));

	for (int i = 0; i < 0x20; i ++) {

		/* ~ Write the data to the name table. ~ */
		ppu_write_internal(PPUDATA, palette[i]);

	}

	/* ~ Begin rendering. ~ */
	RENDER();

}

/* ~ Writes data into the desired internal register of the PPU. ~ */
void ppu_write_internal(uint16_t address, uint8_t value) {

	/* ~ Switch the direction of the data bus to output. ~ */
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

	/* ~ Reset the address latch. ~ */
	RESETLATCH();

	/* ~ Latch the address. ~ */
	ppu_write_internal(PPUADDR, hi(address));
	ppu_write_internal(PPUADDR, lo(address));

	/* ~ Write the desired byte into video RAM. ~ */
	ppu_write_internal(PPUDATA, value);

}

/* ~ Reads and returns a value from a register internal to the PPU. ~ */
uint8_t ppu_read_internal(uint16_t address) {

	/* ~ Switch the direction of the data bus to input. ~ */
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

/* ~ Reads and returns a byte from video RAM at the address specified. ~ */
uint8_t ppu_read(uint16_t address) {

	/* ~ Reset the address latch. ~ */
	RESETLATCH();

	/* ~ Latch the address. ~ */
	ppu_write_internal(PPUADDR, hi(address));
	ppu_write_internal(PPUADDR, lo(address));

	/* ~ Read a dummy byte from video RAM. ~ */
	ppu_read_internal(PPUDATA);

	/* ~ Read and return the desired byte from video RAM. ~ */
	return ppu_read_internal(PPUDATA);

}

/* ~ Transfers an OAM DMA block from a specified address into the PPU. ~ */
void ppu_dma(void *source) {

}

/* ~ Loads the pattern tables in from the filesystem and launches the NES emulator. ~ */
int8_t ppu_emulate(char *rom) {

	/* ~ Obtain the base address of the iNES ROM from external memory. ~ */
	fsp _base = fs.data(rom);

	/* ~ Verify that the ROM was opened succesfully. ~ */
	if (!_base) { error.raise(E_FS_NO_LEAF, ""); return -1; }

	/* ~ Use the base address of the iNES ROM to calcuate the address of the PRG-ROM in external memory. ~ */
	_prg_rom = _base + 16;

	/* ~ Use the base address of the PRG-ROM to calculate the address of the pattern tables in external memory. ~ */
	fsp _chr_rom = _prg_rom + NES_PRG_ROM_SIZE;

	/* ~ Begin a continuous read from external memory to pull in each byte of the pattern tables. ~ */
	at45.read(_chr_rom);

	/* ~ Reset the internal address latch. ~ */
	RESETLATCH();

	/* ~ Latch the address of the pattern tables to begin DMA. ~ */
	ppu_write_internal(PPUADDR, 0x0000);
	ppu_write_internal(PPUADDR, 0x0000);

	/* ~ Transfer the pattern tables from the filesystem into video RAM. ~ */
	for (int i = 0; i < NES_CHR_ROM_SIZE; i ++) {
		ppu_write_internal(PPUDATA, at45.get());
	}

	/* ~ End the continuous read that we began earlier. ~ */
	at45.disable();

}
