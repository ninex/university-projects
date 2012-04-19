CC = gcc
CFLAGS = -g -Wall
C_OBJS = test1.o
RM = rm -f
ASM = nasm
ASMFLAGS = -f elf

# *****************************************************************************
# * Definitions for 'prak1'                                                   *
# *****************************************************************************
PRAC1 = prak1
PRAC1_ASM_SOURCE = prak1.asm
PRAC1_C_OBJS = test1.o
PRAC1_ASM_OBJS = prak1.o
PRAC1_OBJS = $(PRAC1_ASM_OBJS) $(PRAC1_C_OBJS)

# *****************************************************************************
# * Definitions for 'prak2'                                                   *
# *****************************************************************************
PRAC2 = prak2
PRAC2_ASM_SOURCE = prak2.asm
PRAC2_C_OBJS = test2.o
PRAC2_ASM_OBJS = prak2.o
PRAC2_OBJS = $(PRAC2_ASM_OBJS) $(PRAC2_C_OBJS)

# *****************************************************************************
# * Definitions for 'prak3'                                                   *
# *****************************************************************************
PRAC3 = prak3
PRAC3_ASM_SOURCE = prak3.asm
PRAC3_C_OBJS = test3.o
PRAC3_ASM_OBJS = prak3.o
PRAC3_OBJS = $(PRAC3_ASM_OBJS) $(PRAC3_C_OBJS)

# *****************************************************************************
# * Definitions for 'prak3'                                                   *
# *****************************************************************************
PRAC4 = prak4
PRAC4_ASM_SOURCE = prak4.asm
PRAC4_C_OBJS = test4.o
PRAC4_ASM_OBJS = prak4.o
PRAC4_OBJS = $(PRAC4_ASM_OBJS) $(PRAC4_C_OBJS)

all: $(PRAC1) $(PRAC2) $(PRAC3) $(PRAC4)

$(PRAC1): $(PRAC1_OBJS)
	$(CC) $(CFLAGS) $(PRAC1_OBJS) -o $@
	
$(PRAC1_ASM_OBJS): $(PRAC1_ASM_SOURCE)
	$(ASM) $(ASMFLAGS) $(PRAC1_ASM_SOURCE)

$(PRAC2): $(PRAC2_OBJS)
	$(CC) $(CFLAGS) $(PRAC2_OBJS) -o $@
	
$(PRAC2_ASM_OBJS): $(PRAC2_ASM_SOURCE)
	$(ASM) $(ASMFLAGS) $(PRAC2_ASM_SOURCE)

$(PRAC3): $(PRAC3_OBJS)
	$(CC) $(CFLAGS) $(PRAC3_OBJS) -o $@
	
$(PRAC3_ASM_OBJS): $(PRAC3_ASM_SOURCE)
	$(ASM) $(ASMFLAGS) $(PRAC3_ASM_SOURCE)

$(PRAC4): $(PRAC4_OBJS)
	$(CC) $(CFLAGS) $(PRAC4_OBJS) -o $@
	
$(PRAC4_ASM_OBJS): $(PRAC4_ASM_SOURCE)
	$(ASM) $(ASMFLAGS) $(PRAC4_ASM_SOURCE)

clean:
	$(RM) *.{o,map,s,bin} *~
	$(RM) $(PRAC1)
	$(RM) $(PRAC2)
	$(RM) $(PRAC3)
	$(RM) $(PRAC4)
