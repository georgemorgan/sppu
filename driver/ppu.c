#define __private_include__

#include "ppu.h"

/* ~ Create a handle for the module. ~ */

fmr_handle handle;

void ppu_configure(void) {
	
	/* ~ Registers this module with the Flipper Message Runtime. Also calls the remote configure function. ~ */
	
	fmr.bind(&handle, fmr_bundle_id_from_string("io.flipper.ppu"));
	
	/* ~ Ensure that we have sucessfully registered the module. ~ */
	
	/* e.g Failed to load PPU module. Reason: No local reference. Build the module and load it onto the device. */
	
	if (!handle) printf("Failed to load PPU module. Reason: %s\n\n", error.message());
	
}

void ppu_load(void *source, uint16_t length) {
	
	/* ~ Resolves the new address of the data by moving it into the device's memory. ~ */
	
	void *resolved = fmr.resolve(source, length);
	
	/* e.g Failed to load PPU data. Reason: Not enough memory. */
	
	if (!resolved) printf("Failed to resolve PPU data. Reason: %s", error.message());
	
	/* ~ Invokes the function on the device. ~ */
	
	uint32_t ret = fmr.invoke(handle, _ppu_load, 0, resolved, length);
	
}

void ppu_write(void *address, uint8_t value) {
	
	
	
}

uint8_t ppu_read(void *address) {
	
	return 0;
	
}

void ppu_dma(void *source) {
	
	fmr.invoke(handle, _ppu_dma, 0);
	
}