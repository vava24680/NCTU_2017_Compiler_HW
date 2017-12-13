LEX=flex
CC=gcc
SCANNER_SRC=standard-pascal-no-yacc.l
SCANNER_C=lex.yy.c
lex.yy.c:
	$(LEX) $(SCANNER_SRC)
compile:lex.yy.c
	gcc $(SCANNER_C) -lfl
clean:
	rm $(SCANNER_C) *.out
