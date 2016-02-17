#ifndef __cpu_h__
#define __cpu_h__

/* ~ Include all of the standard library amenities. ~ */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* ~ Include all of the types exposed by the Flipper Toolbox. ~ */
#include <flipper/types.h>

/* ~ Define macros for miscelaneous constants. ~ */
#define NES_RAM_SIZE 0x800
#define NES_ROM_SIZE 0x8000

/* ~ Declare a variable that can be used to keep track of the base address of PRG-ROM in the filesystem. ~ */
fsp _prg_rom;

/* ~ Global functions. ~ */
uint8_t nes_cpu_read(uint16_t address);
void nes_cpu_write(uint16_t address, uint8_t value);

#endif
