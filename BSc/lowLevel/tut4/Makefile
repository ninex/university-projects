NAME = tut4
CC = gcc
CFLAGS = -g -Wall
C_OBJS = test4.o
RM = rm -f
ASM = nasm
ASMFLAGS = -f elf
ASM_SOURCE = tut4.asm
ASM_OBJS = tut4.o
OBJS = $(ASM_OBJS) $(C_OBJS)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(ASM_OBJS): $(ASM_SOURCE)
	$(ASM) $(ASMFLAGS) $(ASM_SOURCE) 

clean:
	$(RM) *.{o,map,s,bin} *~
	$(RM) $(NAME)
