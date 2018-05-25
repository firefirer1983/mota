CC = gcc
CXX = g++
LD = ld
OBJCOPY = objcopy
OBJDUMP = objdump
SIZE = size
NM = nm
AR = ar
RANLIB = ranlib
STRIP = strip
READELF = readelf
PRELINK = prelink

.PHONY: clean all

BASE_DIR := mota/base
MDK_DIR  := mota/mdk

all:
	make -C $(BASE_DIR)
	make -C $(MDK_DIR)

clean:
	make -C $(BASE_DIR)
	make -C $(MDK_DIR)
