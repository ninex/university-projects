NAME = jaar
CXX = g++
CC = gcc
RM = rm
FLAGS = -Wall -W
INCPATH = -I/usr/lib/qt3/mkspecs/default -I. -I. -I$(QTDIR)/include
LIBS = -L$(QTDIR)/lib -L/usr/X11R6/lib -L. -lqt-mt -lXext -lX11 -lm
UIC = $(QTDIR)/bin/uic
MOC = $(QTDIR)/bin/moc 
###### Files

HEADERS = 
SOURCES = mpeg2dec.c display.c getbits.c getblk.c gethdr.c getpic.c getvlc.c idct.c idctref.c\
 motion.c recon.c spatscal.c store.c subspic.c systems.c verify.c
OBJS = 	mpeg2dec.o display.o getbits.o getblk.o gethdr.o getpic.o getvlc.o idct.o idctref.o\
 motion.o recon.o spatscal.o store.o subspic.o systems.o verify.o
FORMS = interface.ui
UICDECLS = 
UICIMPLS = 
SRCMOC   = 
OBJMOC =
DIST	   = qt.pro
QMAKE_TARGET = qt
DESTDIR  = 
TARGET   = qt

####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(FLAGS) $(INCPATH) -o $@ $<
.c.o:
	$(CC) -c $(FLAGS)  $(INCPATH) -o $@ $<

####### Compile
all:$(NAME)

$(NAME): $(OBJS)
	$(CXX) $(LIBS) $(FLAGS) $(OBJS) $(OBJMOC) -o $(NAME)
	

clean:
	$(RM) *.o
	$(RM) $(UICDECLS)
	$(RM) $(UICIMPLS)
	$(RM) $(SRCMOC)
