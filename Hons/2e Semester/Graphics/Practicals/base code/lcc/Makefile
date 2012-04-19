# Wedit Makefile for project Base
SRCDIR="f:\hons\graphics\base code"
CFLAGS=-IC:\lcc\include  -g2 
CC=$(LCCROOT)\bin\lcc.exe
LINKER=$(LCCROOT)\bin\lcclnk.exe
OBJS=\
	lesson1.obj

LIBS=
EXE="base.exe"

$(EXE):	$(OBJS) Makefile
	$(LINKER)  -subsystem console -o $(SRCDIR)\"lcc\base.exe" $(OBJS) $(LIBS)

# Build lesson1.c
LESSON1_C=\

lesson1.obj: $(LESSON1_C) $(SRCDIR)\"lesson1.c"
	$(CC) -c $(CFLAGS) $(SRCDIR)\"lesson1.c"

link:
	$(LINKER)  -subsystem console -o $(SRCDIR)\"lcc\base.exe" $(OBJS) $(LIBS)

clean:
	del $(OBJS) base.exe"
