cflags = -std=c99 -Os -mcpu=arm7tdmi -mthumb -mthumb-interwork -mfloat-abi=soft -nostdlib -ffreestanding -fomit-frame-pointer -fpic

flags = $(cflags) -D__device_compilation__ -DLSB_FIRST -DEXEC6502

targets = ppu/*.c ../common.c memory.s cpu/*.c cpu/M6502/*.c -I cpu/M6502 cpu/thumb.s

includes = -I .. -I . -I /usr/local/include -I /usr/local/include/flipper

all:

	# ~ Compile individual targets. ~ #

	arm-elf-gcc-4.7 $(flags) -c $(targets) $(includes)

	# ~ Link objects. ~ * #

	arm-elf-gcc-4.7 $(cflags) -Wl,-T dl.ld *.o -o ppu.elf

	# ~ Dump loadable. ~ #

	arm-elf-objcopy -O binary ppu.elf ppu.bin

dump:

	arm-elf-objdump -D ppu.elf

load: all

	flipper load io.flipper.sppu ppu.bin

clean:

	rm -rf *.elf *.o *.bin
