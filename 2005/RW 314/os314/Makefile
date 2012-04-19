#
# Makefile for CS314 Project
# Jacques Eloff (eloff@cs.sun.ac.za)
# University of Stellenbosch
# 10.02.2005

NAME = os314

# C compiler
CC = gcc

# DDEBUG_I386    - enable debug output for i386.c
# DDEBUG_I8259   - enable debug output for i8259.c
# DDEBUG_I8253   - enable debug output for i8253.c
# DDEBUG_MEM     - enable debug output for mem.c
# DDEBUG_FLOPPY  - enable debug output for floppy.c
# DDEBUG_PROCESS - enable debug output for process.c
# DDEBUG_LOADER  - enable debug output for loader.c
# DDEBUG_TTY     - enable debug output for tty.c
DEBUG_FLAGS = -DDEBUG_I8259 -DDEBUG_MEM \
              -DDEBUG_LOADER -DDEBUG_PROCESS  -DDEBUG_TTY
CFLAGS = $(DEBUG_FLAGS) -W -Wall -g -minline-all-stringops
OBJS = boottable.o trace.o i386.o i8259.o exceptions.o i8253.o \
       init.o stdlib.o mem.o keyboard.o memtest.o\
       floppy.o floppytest.o cmos.o dma.o tty.o sys.o\
       process.o loader.o random.o comms.o
# Linker flags
LDFLAGS = -static -nostdlib --no-strip-discarded
RM = rm -f

ASM = nasm
ASMFLAGS = -f elf
ASM_OBJS = start.o

TOPDIR := $(shell /bin/pwd)

all: $(NAME)

$(NAME): $(OBJS)
	./gen_script $(NAME).ld $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-T,$(NAME).ld -Wl,-Map,$(NAME).map -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $<

clean:
	$(RM) *.{o,map,s,bin} *~
	$(RM) $(NAME)
