.PHONY: all

cc = gcc

targets = sppu.c $(shell find driver -path driver/device -prune -o -name '*.c' -print) 

includes = driver/ $(FLIPPERSDK)/include

libraries = flipper

output = sppu

all:

	$(cc) -Os -std=gnu99 $(foreach dir,$(includes),-I $(dir)) $(foreach library, $(libraries),-l$(library)) $(targets) -o $(output)
