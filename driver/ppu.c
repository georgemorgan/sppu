#define __private_include__
#include "ppu.h"

/* ~ Create a handle for the module. ~ */
static fmr_handle _ppu;

void ppu_configure(void) {
	/* ~ Registers this module with the Flipper Message Runtime. Also calls the remote configure function. ~ */
	_ppu = fmr.bind(fmr_bundle_id_from_string("io.flipper.sppu"));
}

void ppu_load(uint16_t source, uint16_t length) {

}

void ppu_write_internal(uint16_t address, uint8_t value) {
	fmr.invoke(_ppu, _ppu_write_internal, fmr_args(address, value));
}

void ppu_write(uint16_t address, uint8_t value) {
	fmr.invoke(_ppu, _ppu_write, fmr_args(address, value));
}

uint8_t ppu_read_internal(uint16_t address) {
	return fmr.invoke(_ppu, _ppu_read_internal, fmr_args(address));
}

uint8_t ppu_read(uint16_t address) {
	return fmr.invoke(_ppu, _ppu_read, fmr_args(address));
}

void ppu_dma(void *source) {
	fmr.invoke(_ppu, _ppu_dma, fmr_args(source));
}

int8_t ppu_emulate(char *rom) {

	/* ~ Convert the path string into the checksummed basename file identifier the module will expect. ~ */
	uint16_t key = checksum(rom, strlen(rom));

	/* ~ Launch the emulator. ~	*/
	int8_t err = fmr.invoke(_ppu, _ppu_emulate, fmr_args(key));
	if (err == -1) { error.handle(); }

	return err;

}
