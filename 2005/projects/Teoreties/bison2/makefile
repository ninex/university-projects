parse: lex.yy.c tiny.tab.c
	gcc lex.yy.c tiny.tab.c -lfl -o parse

lex.yy.c: tiny.l
	flex tiny.l

tiny.tab.c: tiny.y
	bison -vd tiny.y
