#define __private_include__

#include "ppu.h"

/* ~ Create a handle for the module. ~ */

static fmr_handle handle;

void ppu_configure(void) {
	
	/* ~ Registers this module with the Flipper Message Runtime. Also calls the remote configure function. ~ */
	
	handle = fmr.bind(fmr_bundle_id_from_string("io.flipper.sppu"));
	
	/* ~ Ensure that we have sucessfully registered the module. ~ */
	
	/* e.g Failed to load PPU module. Reason: No local reference. Build the module and load it onto the device. */
	
	if (!handle) printf("Failed to load PPU module. Reason: %s\n\n", error.message());
	
}

void ppu_load(uint16_t source, uint16_t length) {
	
	/* ~ Resolves the new address of the data by moving it into the device's memory. ~ */
	
	//uint16_t resolved = fmr.resolve(source, length);
	
	/* e.g Failed to load PPU data. Reason: Not enough memory. */
	
	//if (!resolved) printf("Failed to resolve PPU data. Reason: %s", error.message());
	
	/* ~ Invokes the function on the device. ~ */
	
	uint16_t ret = fmr.invoke(handle, _ppu_load, fmr_args(source, length));
	
}

void ppu_write_internal(uint16_t address, uint8_t value) {
	
	fmr.invoke(handle, _ppu_write_internal, fmr_args(address, value));
	
}

void ppu_write(uint16_t address, uint8_t value) {
	
	fmr.invoke(handle, _ppu_write, fmr_args(address, value));
	
}

void ppu_fill(uint16_t address, uint8_t value, uint16_t n) {
	
	fmr.invoke(handle, _ppu_fill, fmr_args(address, value, n));
	
}

uint8_t ppu_read_internal(uint16_t address) {
	
	return fmr.invoke(handle, _ppu_read_internal, fmr_args(address));
	
}

uint8_t ppu_read(uint16_t address) {
	
	return fmr.invoke(handle, _ppu_read, fmr_args(address));
	
}

void ppu_dma(void *source) {
	
	fmr.invoke(handle, _ppu_dma, fmr_args(source));
	
}