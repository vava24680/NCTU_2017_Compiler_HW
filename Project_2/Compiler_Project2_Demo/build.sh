bison -d yacc.y
flex lex.l
gcc lex.yy.c yacc.tab.c node.c -ly -lfl -g -o a.out