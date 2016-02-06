flags = -std=c99 -Os -mcpu=arm7tdmi -mfloat-abi=soft -fPIC -nostdlib -ffreestanding -fomit-frame-pointer -fPIC

all:
	
	arm-elf-gcc-4.7 $(flags) -D__device_compilation__ -c ppu.c ../common.c memory.s -I .. -I /usr/local/include -I /usr/local/include/flipper -I "/Development/flipper-toolbox/resources/osx/arm-elf/arm-elf/include"

	arm-elf-gcc-4.7 -fpic -nostdlib -ffreestanding -Wl,-T dl.ld *.o -o ppu.elf

	arm-elf-objcopy -O binary ppu.elf ppu.bin

dump:

	arm-elf-objdump -D ppu.elf

load: all

	flipper load io.flipper.sppu ppu.bin

clean:

	rm -rf *.elf *.o *.bin
