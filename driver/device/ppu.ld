MEMORY {

	LOADABLE (rwx)  : ORIGIN = 0x00000000, LENGTH = 64K

}

SECTIONS {

	. = ORIGIN(LOADABLE);

	.fdl : { * (.fdl); }

	.text : { *(.got .got.*); * (.text .text.*); *(.rodata .rodata.*); }

}