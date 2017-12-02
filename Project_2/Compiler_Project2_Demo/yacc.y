%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

void yyerror(const char *str);
int yywrap();

struct nodeType *ASTROOT;

%}

%union
{
    struct nodeType * node;
    int number;
    char *string;
}

%token <string> ID
%token <number> NUMBER
%token <node> PLUS
%token <node> MINUS
%token <node> MULTIPLY
%token <node> DIVIDE
%token <node> SEMICOLON

%type <node> program statement expression term

%left PLUS MINUS
%left MULTIPLY DIVIDE

%%

program:
        program statement {
            addChild($1, $2);
            $$ = $1;
            printf("Reduction (program -> program statement)\n");
        }
    |   statement {
            $$ = newNode(NODE_PROGRAM);
            addChild($$, $1);
            ASTROOT = $$;
            printf("Reduction (program -> statement)\n");
        }
    ;

statement:
        expression SEMICOLON {
            // $$ = $1;
            $$ = newNode(NODE_STMT);
            addChild($$, $1);
			addChild($$, newNode(NODE_SEMICOLON));
            printf("Reduction (statement -> expression SEMICOLON)\n");
        }
    ;

expression:
        term {
            // $$ = $1;
            $$ = newNode(NODE_TERM);
            addChild($$, $1);
            struct nodeType* expr = $$;
            $$ = newNode(NODE_EXPR);
            addChild($$, expr);
            printf("Reduction (expression -> term)\n");
        }
    |   expression PLUS expression {
        // $$ = $1 + $3;
        $$ = newNode(NODE_EXPR);
        addChild($$, $1);
        addChild($$, newNode(OP_ADD));
        addChild($$, $3);
        printf("Reduction (expression -> expression PLUS expression)\n");
        }
    |   expression MINUS expression {
        // $$ = $1 - $3;
        $$ = newNode(NODE_EXPR);
        addChild($$, $1);
        addChild($$, newNode(OP_SUB));
        addChild($$, $3);
        printf("Reduction (expression -> expression MINUS expression)\n");
        }
    |   expression MULTIPLY expression {
        // $$ = $1 * $3;
        $$ = newNode(NODE_EXPR);
        addChild($$, $1);
        addChild($$, newNode(OP_MUL));
        addChild($$, $3);
        printf("Reduction (expression -> expression MULTIPLY expression)\n");
        }
    |   expression DIVIDE expression {
        // $$ = $1 / $3;
        $$ = newNode(NODE_EXPR);
        addChild($$, $1);
        addChild($$, newNode(OP_DIV));
        addChild($$, $3);
        printf("Reduction (expression -> expression DIVIDE expression)\n");
        }
    ;

term:
        NUMBER {
            // $$ = $1;
            $$ = newNode(NODE_INT);
            $$->iValue = $1;
            // addChild($$, $1);
            printf("Reduction (term -> NUMBER)\n");
        }
    ;



%%

void yyerror(const char *str)
{
    fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
    return 1;
}

int main()
{
    yyparse();
    printf("-----------------------------------------------\n");
    printTree(ASTROOT, 0);
    return 0;
}