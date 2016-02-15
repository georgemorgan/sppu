#define __private_include__
#include "ppu.h"

#ifdef __device_compilation__

__attribute__((section(".fdl")))

#endif

const struct _ppu ppu = {
	ppu_configure,
	ppu_load,
	ppu_write_internal,
	ppu_write,
	ppu_fill,
	ppu_read_internal,
	ppu_read,
	ppu_dma
};
