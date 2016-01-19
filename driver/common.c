#define __private_include__

#include "ppu.h"

const struct _ppu ppu = {
	
	ppu_configure,
	
	ppu_load,
	
	ppu_write,
	
	ppu_read,
	
	ppu_dma
	
};