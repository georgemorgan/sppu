.PHONY: all

cc = gcc

targets = sppu.c $(shell find driver -path driver/device -prune -o -name '*.c' -print) 

includes = driver/

output = sppu

all:

	$(cc) -Os -std=gnu99 $(foreach dir,$(includes),-I $(dir)) $(targets) -o $(output)
