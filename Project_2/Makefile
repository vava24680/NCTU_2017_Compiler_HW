LEX=flex
YACC=bison
CC=gcc
SCANNER_SRC = scanner.l
PARSER_SRC = parser.y
PARSER_H = y.tab.h
SCANNER_C = lex.yy.c
PARSER_C = y.tab.c


#################basic env prepare############
all: compile

.PHONY: clean run

y.tab.c:
	@$(YACC) -d -y $(PARSER_SRC)

lex.yy.c: y.tab.h
	@$(LEX) $(SCANNER_SRC)

compile: y.tab.c lex.yy.c
	@$(CC) node.c lex.yy.c y.tab.c -ly -lfl -lm -g -o parser

clean:
	@-rm -f ${SCANNER_C} ${PARSER_C} ${PARSER_H} parser result

run: # Usage: make run INPUT=<file path>
	@./parser ${INPUT} > result

#####################run all###################
TARGET=$(addprefix Test_result/, $(basename $(notdir $(wildcard Test_files/*.p))))

.PHONY:	run_all clean_all

run_all: run_dir compile ${TARGET}

Test_result/%: Test_files/%.p
	@-./parser $< > $@_result.txt
	@echo $@_result.txt
	@grep Parsing $@_result.txt
	@echo ""

run_dir:
	@mkdir -p Test_result

clean_all:
	@rm -rf Test_result
	@rm -f parser ${SCANNER_C} ${PARSER_C} ${PARSER_H}
