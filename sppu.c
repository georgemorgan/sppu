/*

	Standalone Pixel Processing Unit (SPPU)

	This program interfaces with the SPPU hardware to draw data.

*/

#include "sppu.h"

#include <flipper/flipper.h>

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
	
	/* ~ Get the status register. ~ */

//	ppu.write(12, 0xde);
//	
//	uint32_t status = ppu.read(12);
//	
//	printf("\nThe VRAM value is 0x%02x\n\n", (uint8_t)status);

	
	/* ~ Write a one to address 7 of the PPU. ~ */
	
	for (int i = 0; i < 100; i ++) {
		
		ppu.write(0x2000 + i, i);
		
	}

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