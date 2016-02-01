/*

	Standalone, Pixel Processing Unit (SPPU)

	This program interfaces with the SPPU hardware to draw data.

*/

#include "sppu.h"

#include <flipper/flipper.h>

#define __private_include__

#include <ppu.h>

#include <unistd.h>

struct _operation commands[] = {

	{ "help", "Displays this help message.", sppu_help },
	
	{ "load", "Transfers a .ppu (ppu dump) into the PPU's SRAM.", sppu_load },
	
	{ "clear", "Clears the PPU's frame buffer. (Blanks the screen.)", sppu_clear },
	
	{ "print", "Displays a string on the display using the first character palette.", NULL },

	{ "emulate", "Launches an internal NES emulator and pipes video data to the real PPU.", NULL }
	
};

void parse_arguments(int argc, char *argv[]) {
	
	int op_count = sizeof(commands) / sizeof(struct _operation);
	
	for (int i = 0; i < op_count; i ++) {
		
		if (!strcmp(argv[0], commands[i].key)) { commands[i].handler(-- argc, ++ argv); return; }
			
	}
	
}

void set_tile(unsigned int x, unsigned int y, uint8_t val)
{
	unsigned int ea = 0x2000 + (y * 32) + x;
	ppu.write(ea, val);
}

void ppuprint(unsigned int x, unsigned int y, const char *msg)
{
	while (*(msg++))
	{
		set_tile(x++, y, *msg - 'A');
	}
}

int main(int argc, char *argv[]) {
	
	printf("\n");
	
	/* ~ Print usage information if no argument has been passed. ~ */
	
	if (argc < 2) {
		
		printf("Usage: sppu [help | load | clear | print | emulate]\n\n");
		
		return 0;
		
	}
	
	/* ~ Attatch a Flipper device. ~ */
	
	flipper.attach(FLIPPER_SOURCE_USB);
	
	/* ~ Initialize the PPU module. ~ */
	
	ppu.configure();
	
//	/* ~ Get the status register. ~ */
//
//	ppu.write(0x2005, 0xde);
//	
//	uint16_t status = ppu.read(0x2005);
//	
//	printf("\nThe VRAM value is 0x%08x\n\n", status);

	ppu.load(0x2000, 0);
	//ppuprint(3, 3, "HEY DINGUS");
	//set_tile(2, 2, 0x30);
	
	//ppu.write(0x3F00, 0x22);
	//ppu.write(0x3F03, 0x29);
	/*
	for (int i = 0; i < 16; i++)
	{
		ppu.write(0x3F00 + i, 0x2E);
	}
	
	ppu.write(0x3F03, 0x30);
	ppu.write(0x3F02, 0x2A);*/

	// ppu.fill(0x2000, 0x2F, 2);
	
	/*
	for (int i = 0; i < strlen("HEY DINGUS"); i++)
	{
		set_tile(3 + i, 2, 0x31);
		set_tile(3 + i, 4, 0x41);
	}
	set_tile(2 + strlen("HEY DINGUS"), 2, 0x32);
	set_tile(2, 3, 0x40);
	set_tile(2 + strlen("HEY DINGUS"), 3, 0x42);*/
	
	return 0;
	
//	for (int i = 0; i < 4; i ++) {
//		
//		ppu.write_internal(PPUCTRL, i);
//		
//		printf("Name table %i selected.\n", i);
//		
//		usleep(1000000);
//		
//	}
	
//	char pal[] = { 0x0F, 0x0F, 0x0F, 0x26, 0x0F, 0x16, 0x12, 0x30, 0x0F, 0x16, 0x2C, 0x30, 0x0F, 0x16, 0x2C, 0x30, 0x00, 0x0F, 0x0F, 0x26, 0x00, 0x16, 0x12, 0x30, 0x00, 0x16, 0x2C, 0x30, 0x00, 0x16, 0x2C, 0x30 };
//	
//	PAUSE();
//	
//	for (int i = 0; i < PALETTESIZE; i ++) {
//		
//		ppu.write(PALETTEADDR + i, pal[i]);
//		
//		printf("Wrote to palette.\n");
//		
//		usleep(10000);
//		
//	}
//	
//	RENDER();
	
	return 0;
	
	/* ~ Call the handlers for each argument. ~ */
	
	//parse_arguments(-- argc, ++ argv);
	
	printf("\n");
	
	return 0;
	
}

void sppu_help(OPERATION_PARAMETERS) {
	
	printf(" sppu\n");
	
	int op_count = sizeof(commands) / sizeof(struct _operation);
	
	for (int i = 0; i < op_count; i ++) {
		
		if (i < op_count - 1) printf("├"); else printf("└");
			
		printf("── %s\n%s   └── %s\n", commands[i].key, (i < op_count - 1) ? "│" : " ", commands[i].help);
		
		
	}
	
}

/* ~ THE COOL STUFF :) ~ */

void sppu_load(OPERATION_PARAMETERS) {
	
	if (argc < 1) {
		
		printf("You must specify a .ppu file to load.\n");
		
		return;
		
	}
	
	printf("Loading from .ppu: %s\n\n", argv[0]);
	
	/* ~ Load and open the file. ~ */
	
	FILE *ppu_f = fopen(argv[0], "rb");
	
	if (!ppu_f) {
		
		fprintf(stderr, "Can't open PPU file.\n");
		
		return;
		
	}
	
	void *source;
	
	char c;
	
	while ((c = fgetc(ppu_f)) != EOF) { printf("0x%00xffx ", c); }
	
	printf("\n");
	
	/* ~ Load the data into the PPU. ~ */
	
	//ppu.load(source, 8);
	
	
}

void sppu_clear(OPERATION_PARAMETERS) {
	
	ppu.dma(0);
	
}