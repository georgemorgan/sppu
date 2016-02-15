#ifndef __cpu_h__

#define __cpu_h__

#include <stdio.h>

#include <stdint.h>

#include <stdlib.h>

#define NES_RAM_SIZE 0x800

#define NES_ROM_SIZE 0x8000

/* ~ Global functions. ~ */

uint8_t nes_cpu_read(uint16_t address);

void nes_cpu_write(uint16_t address, uint8_t value);

#endif