/*

	Standalone, Pixel Processing Unit (SPPU)

	This program interfaces with the SPPU hardware to draw data.

*/

#include "sppu.h"

struct _operation commands[] = {
	{ "help", "Displays this help message.", sppu_help },
	{ "load", "Transfers a .ppu (ppu dump) into the PPU's SRAM.", sppu_load },
	{ "clear", "Clears the PPU's frame buffer. (Blanks the screen.)", sppu_clear },
	{ "print", "Displays a string on the display using the first character palette.", NULL },
	{ "emulate", "Launches an internal NES emulator and pipes video data to the real PPU.", sppu_emulate }
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

//	/* ~ Attatch a Flipper device. ~ */
//	flipper.attach(FLIPPER_SOURCE_USB);
//
//	/* ~ Initialize the PPU module. ~ */
//	ppu.configure();
//
//	if (!strcmp(argv[1], "clear")) {
//		ppu.load(0, 0);
//	}
//
//	else if (!strcmp(argv[1], "background")) {
//		PAUSE();
//		ppu.write(0x3F00, atoi(argv[2]));
//		RENDER();
//	}
//
//	else if (!strcmp(argv[1], "seltables")) {
//		/* ~ Select the name table. ~ */
//		uint8_t bnt = atoi(argv[2]) & 0x3;
//		uint8_t bt = atoi(argv[3]) & 0x1;
//
//		ppu.write_internal(PPUCTRL, (bt << 4) | bnt);
//
//		printf("\nSelected base name table %i with background table %i.\n\n", bnt, bt);
//	}
//
//	else if (!strcmp(argv[1], "writepal")) {
//		/* ~ Select the name table. ~ */
//		uint8_t offset = atoi(argv[2]) & 0xF;
//		uint8_t value = atoi(argv[3]) & 0xFF;
//
//		for (int i = 0; i < 100; i ++) {
//			ppu.write(0x3F00 + offset + i, value);
//			usleep(1000000);
//		}
//
//		printf("\nWrote 0x%02x to 0x%04x (%i).\n\n", value, 0x3f00 + offset, offset);
//	}
//
//	else if (!strcmp(argv[1], "color")) {
//		char pal[] = { 0x0F, 0x0F, 0x0F, 0x26, 0x0F, 0x16, 0x12, 0x30, 0x0F, 0x16, 0x2C, 0x30, 0x0F, 0x16, 0x2C, 0x30, 0x00, 0x0F, 0x0F, 0x26, 0x00, 0x16, 0x12, 0x30, 0x00, 0x16, 0x2C, 0x30, 0x00, 0x16, 0x2C, 0x30 };
//
//		PAUSE();
//
//		for (int i = 0; i < 0x10; i ++) {
//			ppu.write(PALETTEADDR + i, pal[i]);
//			printf("Derp\n");
//			usleep(100000);
//		}
//
//		printf("Wrote to palette.\n");
//	}
//
//	else if (!strcmp(argv[1], "print")) {
//
//		nes_print(argv[2]);
//
//	}
//
//	else if (!strcmp(argv[1], "scroll")) {
//		/* ~ Select the name table. ~ */
//		uint8_t x = atoi(argv[2]) & 0xFF;
//		uint8_t y = atoi(argv[3]) & 0xFF;
//
//		RESETLATCH();
//
//		ppu.write_internal(PPUSCROLL, x);
//		ppu.write_internal(PPUSCROLL, y);
//
//		printf("\nWrote scroll x: %i y: %i.\n\n", x, y);
//	}

	/* ~ Call the handlers for each argument. ~ */
	parse_arguments(-- argc, ++ argv);

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

	char c;
	while ((c = fgetc(ppu_f)) != EOF) { printf("0x%00xffx ", c); }

	printf("\n");
}

void sppu_clear(OPERATION_PARAMETERS) {

}

void sppu_emulate(OPERATION_PARAMETERS) {
	/* ~ Load the iNES ROM. ~ */
	printf("Transferring iNES ROM '%s'.\n\n", argv[0]);

	/* ~ Upload the ROM to the device's filesystem. ~ */
	fs.upload(argv[0]);

	/* ~ Launch the emulator with the given ROM. ~ */
	ppu.emulate(argv[0]);

}
