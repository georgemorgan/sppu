#ifndef sppu_h
#define sppu_h

/*
 * Standalone Picture Processing Unit (sppu)
 *
 * This console application uses Flipper to interface with a standalone
 * Picture Processing Unit from the original Nintendo Entertainment
 * System. It can be used to write data directly into VRAM as well
 * as the PPU's internal registers. Facilities also exist for loading
 * pattern table information into the PPU from an iNES ROM file. This
 * functionality alone can be used to display VRAM dumps from an emulator.
 * An NES emulator for Flipper hardware is currently in development and can
 * be used in conjunction with sppu to launch an emulated instance of an NES
 * ROM for display on authentic NES video synthesis hardware.
 *
 * Usage: sppu [help | load | clear | print | emulate]
 */

/* ~ Include the standard library headers. ~ */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ~ Include the Flipper header and PPU driver with private definitions. ~ */
#define __private_include__
#include <flipper.h>
#include <ppu.h>

#define OPERATION_PARAMETERS int argc, char *argv[]

struct _operation {
	/* ~ Arguments with this key are forwarded to the handler. ~ */
	char *key;

	/* ~ The help message for the operation. ~ */
	char *help;

	/* ~ The handler of the operation. Accepts the remaining argument count and list. ~ */
	void (* handler)(OPERATION_PARAMETERS);
};

/* ~ Define functions. ~ */
void sppu_help(OPERATION_PARAMETERS);
void sppu_load(OPERATION_PARAMETERS);
void sppu_clear(OPERATION_PARAMETERS);
void sppu_emulate(OPERATION_PARAMETERS);

#endif
