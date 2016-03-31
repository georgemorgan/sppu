.PHONY: all

cc = gcc

targets = sppu.c $(shell find driver -path driver/device -prune -o -name '*.c' -print)

includes = driver /usr/local/include

libraries = flipper

output = sppu

all:

	$(cc) -Os -std=gnu99 $(foreach dir,$(includes),-I $(dir)) -L /usr/local/lib -lflipper $(targets) -o $(output)

	$(MAKE) -C driver/device

dump:

	$(MAKE) -C driver/device dump

load:

	$(MAKE) -C driver/device load

clean:

	rm -rf sppu

	$(MAKE) -C driver/device clean
