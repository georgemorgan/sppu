#define __private_include__

#include "ppu.h"

/* ~ Create a handle for the module. ~ */

static fmr_handle handle;

void ppu_configure(void) {
	
	/* ~ Registers this module with the Flipper Message Runtime. Also calls the remote configure function. ~ */
	
	fmr.bind(&handle, fmr_bundle_id_from_string("io.flipper.ppu"));
	
	/* ~ Ensure that we have sucessfully registered the module. ~ */
	
	/* e.g Failed to load PPU module. Reason: No local reference. Build the module and load it onto the device. */
	
	if (!handle) printf("Failed to load PPU module. Reason: %s\n\n", error.message());
	
}

void ppu_load(uint32_t source, uint16_t length) {
	
	/* ~ Resolves the new address of the data by moving it into the device's memory. ~ */
	
	//uint32_t resolved = fmr.resolve(source, length);
	
	/* e.g Failed to load PPU data. Reason: Not enough memory. */
	
	//if (!resolved) printf("Failed to resolve PPU data. Reason: %s", error.message());
	
	/* ~ Invokes the function on the device. ~ */
	
	uint32_t ret = fmr.invoke(handle, _ppu_load, 4, fmr_argument(source), fmr_argument(length));
	
}

void ppu_write(uint32_t address, uint8_t value) {
	
	printf("Writing!\n");
	
	fmr.invoke(handle, _ppu_write, 4, fmr_argument(address), fmr_argument(value));
	
}

uint8_t ppu_read(uint32_t address) {
	
	return 	fmr.invoke(handle, _ppu_read, 2, fmr_argument(address));
	
}

void ppu_dma(void *source) {
	
	fmr.invoke(handle, _ppu_dma, 0);
	
}