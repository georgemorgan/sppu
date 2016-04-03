cflags = -std=c99 -Os -mcpu=arm7tdmi -mthumb -mthumb-interwork -mfloat-abi=soft -nostdlib -ffreestanding -fomit-frame-pointer -fpic

flags = $(cflags) -D__device_compilation__ -DLSB_FIRST -DEXEC6502

targets = *.c ../common.c memory.s # cpu/*.c cpu/*.s cpu/M6502/*.c

includes = -I .. -I . -I /usr/local/include -I /usr/local/include/flipper -I cpu/M6502

all:

	# ~ Compile individual targets. ~ #
	arm-elf-gcc $(flags) $(includes) -c $(targets)

	# ~ Link objects. ~ * #
	arm-elf-gcc $(cflags) -Wl,-T ppu.ld *.o -o ppu.elf

	# ~ Dump loadable. ~ #
	arm-elf-objcopy -O binary ppu.elf ppu.bin

dump:

	arm-elf-objdump -D ppu.elf

install:

	flipper install io.flipper.sppu ppu.bin

clean:

	rm -rf *.elf *.o *.bin
