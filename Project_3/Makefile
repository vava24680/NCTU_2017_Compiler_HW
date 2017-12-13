LEX=flex
YACC=bison
CC=g++
SCANNER_SRC = scanner.l
PARSER_SRC = parser.y
PARSER_H = parser.tab.h
SCANNER_C = lex.yy.c
PARSER_C = parser.tab.c
SYMBOL_CPP = symbol_table.cpp

#################basic env prepare############
all: compile

.PHONY: clean run

y.tab.c:
	@$(YACC) -d $(PARSER_SRC)

lex.yy.c: parser.tab.h
	@$(LEX)  $(SCANNER_SRC)

compile: y.tab.c lex.yy.c
	@$(CC) node.cpp lex.yy.c parser.tab.c symbol_table.cpp -ly -lfl -lm -g

clean:
	@-rm -f ${SCANNER_C} ${PARSER_C} ${PARSER_H} a.out
###################Run all###################
TARGET=$(addprefix Test_result/, $(basename $(notdir $(wildcard Test_files/*.p))))

.PHONY:	run_all clean_all

run_all: compile ${TARGET}

Test_result/%: Test_files/%.p
	@-./a.out $< > $@_result.txt
	@echo $@_result.txt