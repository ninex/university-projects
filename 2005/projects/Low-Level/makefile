NAME = compress
CC = gcc
CFLAGS = -g -Wall
RM = rm -f
ASM = nasm 
ASMFLAGS = -f elf
ASM_SOURCE1 = main.asm
ASM_SOURCE2 = huffman.asm
ASM_SOURCE3 = heap.asm
ASM_OBJS1 = main.o
ASM_OBJS2 = huffman.o
ASM_OBJS3 = heap.o
OBJS = $(ASM_OBJS1) $(ASM_OBJS2) $(ASM_OBJS3)
CFILES = main.c heap.c

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(ASM_OBJS1) : $(ASM_SOURCE1)
	$(ASM) $(ASMFLAGS) $(ASM_SOURCE1)
    
$(ASM_OBJS2) : $(ASM_SOURCE2)
	$(ASM) $(ASMFLAGS) $(ASM_SOURCE2)
  
$(ASM_OBJS3) : $(ASM_SOURCE3)
	$(ASM) $(ASMFLAGS) $(ASM_SOURCE3)

clean:
	$(RM) *.{o,map,s,bin,cz} *~
	$(RM) $(NAME)

gcc:
	$(CC) -c $(CFILES)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)