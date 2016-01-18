#ifndef sppu_driver_h

#define sppu_driver_h

/* ~ Declare the virtual interface for the PPU driver. ~ */

extern const struct _ppu {
	
	void (* configure)(void);
	
	void (* load)(void *source, uint16_t length);
	
	void (* write)(void *address, uint8_t value);
	
	uint8_t (* read)(void *address);
	
};

/* ~ Define the private definitions of the driver functions. ~ */

#ifndef private_include

/* ~ Configures the necessary IO to interface with the PPU. ~ */

extern void ppu_configure(void);

/* ~ Loads an entire PPU dump into PPU memory. ~ */

extern void ppu_load(void *source, uint16_t length);

/* ~ Writes to an internal PPU address. ~ */

extern void ppu_write(void *address, uint8_t value);

/* ~ Reads from an internal PPU address. ~ */

extern uint8_t ppu_read(void *address);

/* ~ Simulates the DMA of SPR-RAM. The length is assumed. ~ */

extern void ppu_dma(void *source);

#endif

#endif