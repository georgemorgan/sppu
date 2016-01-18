.PHONY: all

cc = gcc

targets = sppu.c

output = sppu

all:

	$(cc) -Os -std=gnu99 $(targets) -o $(output)