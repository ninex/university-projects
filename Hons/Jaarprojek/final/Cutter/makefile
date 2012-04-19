NAME = cutter
CXX = g++
CC = gcc
RM = rm
FLAGS = -Wall -W 
INCPATH = -I.
LIBS = -L. -lm

###### Files

SOURCES = main.cpp mpeg2dec.cpp display.cpp getbits.cpp getblk.cpp gethdr.cpp getpic.cpp getvlc.cpp idct.cpp idctref.cpp\
 motion.cpp recon.cpp spatscal.cpp store.cpp subspic.cpp systems.cpp verify.cpp split.cpp cut.cpp parse.cpp 
OBJS = main.o mpeg2dec.o display.o getbits.o getblk.o gethdr.o getpic.o getvlc.o idct.o idctref.o\
 motion.o recon.o spatscal.o store.o subspic.o systems.o verify.o split.o cut.o parse.o

####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(FLAGS) $(INCPATH) -o $@ $<
.c.o:
	$(CC) -c $(FLAGS)  $(INCPATH) -o $@ $<

####### Compile
all:$(NAME)

$(NAME): $(OBJS)
	$(CXX) $(LIBS) $(FLAGS) $(OBJS) -o $(NAME)
	

clean:
	$(RM) *.o
	$(RM) $(NAME)
