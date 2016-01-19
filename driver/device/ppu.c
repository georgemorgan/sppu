#include <ppu.h>

#include "atmel.h"

#define PIN 7

void ppu_configure(void) {
	
	
	
}

void ppu_load(void *source, uint16_t length) {
	
	AT91C_BASE_PIOA -> PIO_PER |= (1 << PIN);
	
	AT91C_BASE_PIOA -> PIO_OER |= (1 << PIN);
	
	AT91C_BASE_PIOA -> PIO_SODR |= (1 << PIN);
	
}

void ppu_write(void *address, uint8_t value) {
	
	
	
}

uint8_t ppu_read(void *address) {
	
	return 0;
	
}

void ppu_dma(void *source) {
	
	AT91C_BASE_PIOA -> PIO_CODR |= (1 << PIN);
	
}
