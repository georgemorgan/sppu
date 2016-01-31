llvm_flags = -std=c99 -Os -target armv7-none-elf -march=arm7tdmi -mfloat-abi=soft -fPIC -nostdlib -ffreestanding -fomit-frame-pointer -fPIC

all:
	
	clang $(llvm_flags) -D__device_compilation__ -c ppu.c ../common.c -I .. -I /usr/local/include -I /usr/local/include/flipper --sysroot "/Development/flipper-toolbox/resources/osx/arm-elf/arm-elf" -I "/Development/flipper-toolbox/resources/osx/arm-elf/arm-elf/include"

	arm-elf-gcc-4.7 -fPIC -nostdlib -ffreestanding -fomit-frame-pointer -Wl,-T dl.ld *.o -o ppu.elf

	arm-elf-objcopy -O binary ppu.elf ppu.bin

	arm-elf-objdump -D ppu.elf

load: all

	flipper load io.flipper.ppu ppu.bin

clean:

	rm -rf *.elf *.o *.bin
