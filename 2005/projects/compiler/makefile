
NAME = cpl
FILES = scanner.o compiler.o parser.o error.o symboltable.o codegen.o
#codegen.o
#Available Debug flags are -DDEBUG_PARSER -DPARSER_PRINTING -DDEBUG_SYM -DDEBUG_INTERP -DDEBUG_INTERP_INSTR -DDEBUG_CODEGEN
CFLAGS = -W -Wall 
CC = gcc

all: $(NAME)

$(NAME):$(FILES) 	
	$(CC) $(CFLAGS) $(FILES) -o $(NAME)
	$(CC) $(CFLAGS) interpreter.c codegen.o -o rcp


clean: 
	rm -f *.o 
