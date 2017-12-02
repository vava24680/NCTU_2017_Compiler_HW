
%{
/*
 * grammar.y
 *
 * Pascal grammar in Yacc format, based originally on BNF given
 * in "Standard Pascal -- User Reference Manual", by Doug Cooper.
 * This in turn is the BNF given by the ANSI and ISO Pascal standards,
 * and so, is PUBLIC DOMAIN. The grammar is for ISO Level 0 Pascal.
 * The grammar has been massaged somewhat to make it LALR, and added
 * the following extensions.
 *
 * constant expressions
 * otherwise statement in a case
 * productions to correctly match else's with if's
 * beginnings of a separate compilation facility
 */
#include<stdio.h>
#include<stdlib.h>
#include "node.h"
//#define YYDEBUG 1
extern int yylex(void);
void yyerror(const char *str);
struct nodeType *ASTROOT;
/*NODE** node_ptr_array = NULL;
int ptr_counter = 0;
int ptr_array_size = 0;*/
int yywrap()
{
    return 1;
}

%}

%union sementic{
	struct nodeType* node;
	int integer;
	double realnumber;
	char* id;
	char* string;
}

%start prog

//%token ARRAY DO ELSE END
//%token FUNCTION IF NOT OF
//%token PBEGIN PROCEDURE PROGRAM THEN
//%token VAR WHILE
//%token COLON COMMA
//%token DOTDOT EQUAL GE GT
//%token LBRAC LE LPAREN LT
//%token RBRAC REALNUMBER
//%token RPAREN SEMICOLON SLASH STAR
//%token NOTEQUAL COMMENT SCIENTIFIC_EXP_REAL SCIENTIFIC_EXP_EXPON
//%token NUMBER INT_WORD REAL_WORD
%token <node> ARRAY
%token <node> DO
%token <node> ELSE
%token <node> END
%token <node> FUNCTION
%token <node> IF
%token <node> NOT
%token <node> OF
%token <node> PBEGIN
%token <node> PROCEDURE
%token <node> PROGRAM
%token <node> THEN
%token <node> VAR
%token <node> WHILE
%token <id> IDENTIFIER
%token <node> ASSIGNMENT
%token <node> COLON
%token <node> COMMA
%token <integer> INTEGER
%token <node> DOT
%token <node> DOTDOT
%token <node> EQUAL
%token <node> GE
%token <node> GT
%token <node> LBRAC
%token <node> LE
%token <node> LPAREN
%token <node> LT
%token <node> MINUS
%token <node> PLUS
%token <node> RBRAC
%token <realnumber> REALNUMBER
%token <node> RPAREN
%token <node> SEMICOLON
%token <node> SLASH
%token <node> STAR
%token <node> NOTEQUAL
%token <realnumber> SCIENTIFIC_EXP
%token <node> SCIENTIFIC_EXP_EXPON
%token <string> NUMBER
%token <node> INT_WORD
%token <node> REAL_WORD
%token <node> STRING_WORD

%type <node> prog
%type <node> identifier_list
%type <node> declarations
%type <node> type
%type <node> standard_type
%type <node> subprogram_declarations
%type <node> subprogram_declaration
%type <node> subprogram_head
%type <node> arguments
%type <node> parameter_list
%type <node> optional_var
%type <node> compound_statement
%type <node> optional_statements
%type <node> statement_list
%type <node> statement
%type <node> variable
%type <node> tail
%type <node> procedure_statement
%type <node> expression_list
%type <node> expression
%type <node> simple_expression
%type <node> term
%type <node> factor
%type <node> addop
%type <node> mulop
%type <node> relaop
//%type <node> lambda
%left PLUS MINUS
%left STAR SLASH
%%

prog :
	PROGRAM IDENTIFIER LPAREN identifier_list RPAREN SEMICOLON
	declarations
	subprogram_declarations
	compound_statement DOT
	{
		$$ = newNode(NODE_PROG);
		NODE* temp  = newNode(NODE_IDENTIFIER);
		temp->id = $2;
		addChild($$, newNode(NODE_PROGRAM));
		addChild($$, temp);
		addChild($$, newNode(NODE_LPAREN));
		addChild($$, $4);
		addChild($$, newNode(NODE_RPAREN));
		addChild($$, newNode(NODE_SEMICOLON));
		addChild($$, $7);
		addChild($$, $8);
		addChild($$, $9);
		addChild($$, newNode(NODE_DOT));
		ASTROOT = $$;
		printf("Reduction ( prog -> program identifier LPAREN identifier_list RPAREN SEMICOLON declarations subprogram_declarations compound_statement DOT)\n");
	}
	;

declarations :
	declarations VAR identifier_list COLON type SEMICOLON
	{
		$$ = newNode(NODE_DECLARATIONS);
		addChild($$, $1);
		addChild($$, newNode(NODE_VAR));
		addChild($$, $3);
		addChild($$, newNode(NODE_COLON));
		addChild($$, $5);
		addChild($$, newNode(NODE_SEMICOLON));
		printf("Reduction ( declarations -> declarations VAR identifier_list COLON type SEMICOLON)\n");
	}
	|
		lambda
		{
			$$ = newNode(NODE_DECLARATIONS);
			addChild($$, newNode(NODE_LAMBDA));
			printf("Reduction ( declarations -> lambda\n");
		};

type :
	standard_type
	{
		/*Single*/
		$$ = newNode(NODE_TYPE);
		addChild($$, $1);
		printf("Reduction ( type -> standard type)\n");
	}
	|	ARRAY LBRAC INTEGER DOTDOT INTEGER RBRAC OF type
		{
			$$ = newNode(NODE_TYPE);
			NODE* temp_1 = newNode(NODE_INTEGER);
			NODE* temp_2 = newNode(NODE_INTEGER);
			temp_1->integer_value = $3;
			temp_2->integer_value = $5;
			addChild($$, newNode(NODE_ARRAY));
			addChild($$, newNode(NODE_LBRAC));
			addChild($$, temp_1);
			addChild($$, newNode(NODE_DOTDOT));
			addChild($$, temp_2);
			addChild($$, newNode(NODE_RBRAC));
			addChild($$, newNode(NODE_OF));
			addChild($$, $8);
			printf("Reduction ( type -> ARRAY LBRAC INTEGER DOTDOT INTEGER RBRAC OF type)\n");
		};

standard_type :
	INT_WORD
	{
		/*Single*/
		$$ = newNode(NODE_STANDARD_TYPE);
		addChild($$, newNode(NODE_INT_WORD));
		printf("Reduction ( standard_type -> INT_WORD)\n");
	}
	|
		REAL_WORD
		{
			/*Single*/
			$$ = newNode(NODE_STANDARD_TYPE);
			addChild($$, newNode(NODE_REAL_WORD));
			printf("Reduction ( standard_type -> REAL_WORD)\n");
		}
	|
		STRING_WORD
		{
			/*Single*/
			$$ = newNode(NODE_STANDARD_TYPE);
			addChild($$, newNode(NODE_STRING_WORD));
			printf("Reduction ( standard_type -> STRING_WORD)\n");
		};

subprogram_declarations :
	subprogram_declarations subprogram_declaration SEMICOLON
	{
		$$ = newNode(NODE_SUBDECLARATIONS);
		addChild($$, $1);
		addChild($$, $2);
		addChild($$, newNode(NODE_SEMICOLON));
		printf("Reduction ( subprogram_declarations -> subprogram_declarations subprogram_declaration SEMICOLON)\n");
	}
	|
		{
			$$ = newNode(NODE_SUBDECLARATIONS);
			addChild($$, newNode(NODE_LAMBDA));
			printf("Reduction ( subprogram_declarations -> lambda)\n");
		};

subprogram_declaration :
	subprogram_head declarations compound_statement
	{
		$$ = newNode(NODE_SUBDECLARATION);
		addChild($$, $1);
		addChild($$, $2);
		addChild($$, $3);
		printf("Reduction ( subprogram_declaration -> subprogram_head declarations compound_statement)\n");
	};

subprogram_head :
	FUNCTION IDENTIFIER arguments COLON standard_type SEMICOLON
	{
		$$ = newNode(NODE_SUBPROGRAM_HEAD);
		NODE* temp = newNode(NODE_IDENTIFIER);
		temp->id = $2;
		addChild($$, newNode(NODE_FUNCTION));
		addChild($$, temp);
		addChild($$, $3);
		addChild($$, newNode(NODE_COLON));
		addChild($$, $5);
		addChild($$, newNode(NODE_SEMICOLON));
		printf("Reduction ( subprogram_head -> FUNCTION identifier arguments COLON standard_type SEMICOLON)\n");
	}
	|
		PROCEDURE IDENTIFIER arguments SEMICOLON
		{
			$$ = newNode(NODE_SUBPROGRAM_HEAD);
			NODE* temp = newNode(NODE_IDENTIFIER);
			temp->id = $2;
			addChild($$, newNode(NODE_PROCEDURE));
			addChild($$, temp);
			addChild($$, $3);
			addChild($$, newNode(NODE_SEMICOLON));
			printf("Reduction ( subprogram_head -> PROCEDURE identifier arguments SEMICOLON)\n");
		};

arguments :
	LPAREN parameter_list RPAREN
	{
		$$ = newNode(NODE_ARGUMENTS);
		addChild($$, newNode(NODE_LPAREN));
		addChild($$, $2);
		addChild($$, newNode(NODE_RPAREN));
		printf("Reduction ( arguments -> LPAREN parameter_list RPAREN)\n");
	}
	|
		{
			$$ = newNode(NODE_ARGUMENTS);
			addChild($$, newNode(NODE_LAMBDA));
			printf("Reduction ( arguments -> lambda)\n");
		};

parameter_list :
	optional_var identifier_list COLON type
	{
		$$ = newNode(NODE_PARAM_LIST);
		addChild($$, $1);
		addChild($$, $2);
		addChild($$, newNode(NODE_COLON));
		addChild($$, $4);
		printf("Reduction ( parameter_list -> optional_var identifier_list COLON type)\n");
	}
	|
		optional_var identifier_list COLON type SEMICOLON parameter_list
		{
			$$ = newNode(NODE_PARAM_LIST);
			addChild($$, $1);
			addChild($$, $2);
			addChild($$, newNode(NODE_COLON));
			addChild($$, $4);
			addChild($$, newNode(NODE_SEMICOLON));
			addChild($$, $6);
			printf("Reduction ( parameter_list -> optional_var identifier_list COLON type SEMICOLON parameter_list)\n");
		};

optional_var :
	VAR
	{
		/*Single*/
		$$ = newNode(NODE_OPTIONAL_VAR);
		addChild($$, newNode(NODE_VAR));
		printf("Reduction ( optional_var -> VAR)\n");
	}
	|
		{
			$$ = newNode(NODE_OPTIONAL_VAR);
			addChild($$, newNode(NODE_LAMBDA));
			printf("Reduction ( optional_var -> lambda)\n");
		};

compound_statement :
	PBEGIN optional_statements END
	{
		$$ = newNode(NODE_COMP_STATE);
		addChild($$, newNode(NODE_PBEGIN));
		addChild($$, $2);
		addChild($$, newNode(NODE_END));
		printf("Reduction ( compound_statement -> begin optional_statements end)\n");
	};

optional_statements :
	statement_list
	{
		/*Single*/
		$$ = newNode(NODE_OPTIONAL_STATE);
		addChild($$, $1);
		printf("Reduction ( optional_statements -> statement_list)\n");
	};

statement_list :
	statement
	{
		/*Single*/
		$$ = newNode(NODE_STATEMENT_LIST);
		addChild($$, $1);
		printf("Reduction ( statement_list -> statement)\n");
	}
	|
		statement_list SEMICOLON statement
		{
			$$ = newNode(NODE_STATEMENT_LIST);
			addChild($$, $1);
			addChild($$, newNode(NODE_SEMICOLON));
			addChild($$, $3);
			printf("Reduction ( statement_list -> statement_list SEMICOLON statement)\n");
		};

statement :
	variable ASSIGNMENT expression
	{
		$$ = newNode(NODE_STATEMENT);
		addChild($$, $1);
		addChild($$, newNode(NODE_ASSIGNMENT));
		addChild($$, $3);
		printf("Reduction ( statement -> variable ASSIGNMENT expression)\n");
	}
	|
		procedure_statement
		{
			/*Single*/
			$$ = newNode(NODE_STATEMENT);
			addChild($$, $1);
			printf("Reduction ( statement -> procedure_statement)\n");
		}
	|
		compound_statement
		{
			/*Single*/
			$$ = newNode(NODE_STATEMENT);
			addChild($$, $1);
			printf("Reduction ( statement -> compound_statement)\n");
		}
	|
		IF expression THEN statement ELSE statement
		{
			$$ = newNode(NODE_STATEMENT);
			addChild($$, newNode(NODE_IF));
			addChild($$, $2);
			addChild($$, newNode(NODE_THEN));
			addChild($$, $4);
			addChild($$, newNode(NODE_ELSE));
			addChild($$, $6);
			printf("Reduction ( statement -> IF expression THEN statement ELSE statement)\n");
		}
	|
		WHILE expression DO statement
		{
			$$ = newNode(NODE_STATEMENT);
			addChild($$, newNode(NODE_WHILE));
			addChild($$, $2);
			addChild($$, newNode(NODE_DO));
			addChild($$, $4);
			printf("Reduction ( statement -> WHILE expression DO statement)\n");
		}
	|
		{
			$$ = newNode(NODE_STATEMENT);
			addChild($$, newNode(NODE_LAMBDA));
			printf("Reduction ( statement -> lambda)\n");
		};

variable :
	IDENTIFIER tail
	{
		$$ = newNode(NODE_VARIABLE);
		NODE* temp = newNode(NODE_IDENTIFIER);
		temp->id = $1;
		addChild($$, temp);
		addChild($$, $2);
		printf("Reduction ( variable -> identifier tail)\n");
	};

tail :
	LBRAC expression RBRAC tail
	{
		$$ = newNode(NODE_TAIL);
		addChild($$, newNode(NODE_LBRAC));
		addChild($$, $2);
		addChild($$, newNode(NODE_RBRAC));
		addChild($$, $4);
		printf("Reduction ( tail -> LBRAC expression RBRAC tail)\n");
	}
	|
		{
			$$ = newNode(NODE_TAIL);
			addChild($$, newNode(NODE_LAMBDA));
			printf("Reduction ( tail -> lambda)\n");
		};

procedure_statement :
	IDENTIFIER
	{
		/*Single*/
		$$ = newNode(NODE_PROC_STATE);
		NODE* temp = newNode(NODE_IDENTIFIER);
		temp->id = $1;
		addChild($$, temp);
		printf("Reduction ( procedure_statement -> identifier)\n");
	}
	|
		IDENTIFIER LPAREN expression_list RPAREN
		{
			$$ = newNode(NODE_PROC_STATE);
			NODE* temp = newNode(NODE_IDENTIFIER);
			temp->id = $1;
			addChild($$, temp);
			addChild($$, newNode(NODE_LPAREN));
			addChild($$, $3);
			addChild($$, newNode(NODE_RPAREN));
			printf("Reduction ( procedure_statement -> identifier LPAREN expression_list RPAREN)\n");
		};

expression_list :
	expression
	{
		/*Single*/
		$$ = newNode(NODE_EXPR_LIST);
		addChild($$, $1);
		printf("Reduction ( expression_list -> expression)\n");
	}
	|
		expression_list COMMA expression
		{
			$$ = newNode(NODE_EXPR_LIST);
			addChild($$, $1);
			addChild($$, newNode(NODE_COMMA));
			addChild($$, $3);
			printf("Reduction ( expression_list -> expression_list COMMA expression)\n");
		};

expression :
	simple_expression
	{
		/*Single*/
		$$ = newNode(NODE_EXPR);
		addChild($$, $1);
		printf("Reduction ( expression -> simple_expression)\n");
	}
	|
		simple_expression relaop simple_expression
		{
			$$ = newNode(NODE_EXPR);
			addChild($$, $1);
			addChild($$, $2);
			addChild($$, $3);
			printf("Reduction ( expression -> simple_expression relaop simple_expression)\n");
		};

simple_expression :
	term
	{
		/*Single*/
		$$ = newNode(NODE_SIM_EXPR);
		addChild($$, $1);
		printf("Reduction ( simple_expression -> term)\n");
	}
	|
		simple_expression addop term
		{
			$$ = newNode(NODE_SIM_EXPR);
			addChild($$, $1);
			addChild($$, $2);
			addChild($$, $3);
			printf("Reduction ( simple_expression -> simple_expression addop term)\n");
		};

term :
	factor
 	{
		/*Single*/
		$$ = newNode(NODE_TERM);
		addChild($$, $1);
		printf("Reduction ( term -> factor)\n");
	}
	|
		term mulop factor
		{
			$$ = newNode(NODE_TERM);
			addChild($$, $1);
			addChild($$, $2);
			addChild($$, $3);
			printf("Reduction ( term -> term mulop factor)\n");
		};

factor :
	IDENTIFIER tail
	{
		$$ = newNode(NODE_FACTOR);
		NODE* temp = newNode(NODE_IDENTIFIER);
		temp->id = $1;
		addChild($$, temp);
		addChild($$, $2);
		printf("Reduction ( factor -> identifier tail)\n");
	}
	|
		IDENTIFIER LPAREN expression_list RPAREN
		{
			$$ = newNode(NODE_FACTOR);
			NODE* temp = newNode(NODE_IDENTIFIER);
			temp->id = $1;
			addChild($$, temp);
			addChild($$, newNode(NODE_LPAREN));
			addChild($$, $3);
			addChild($$, newNode(NODE_RPAREN));
			printf("Reduction ( factor -> identifier LPAREN expression_list RPAREN)\n");
		}
	|
		INTEGER
		{
			NODE* temp = newNode(NODE_INTEGER);
			temp->integer_value = $1;
			$$ = newNode(NODE_FACTOR);
			addChild($$, temp);
			printf("Reduction ( factor -> INTEGER)\n");
		}
	|
		addop INTEGER
		{
			$$ = newNode(NODE_FACTOR);
			NODE* temp = newNode(NODE_INTEGER);
			temp->integer_value = $2;
			addChild($$, $1);
			addChild($$, temp);
			printf("Reduction ( factor -> addop INTEGER)\n");
		}
	|
		REALNUMBER
		{
			NODE* temp = newNode(NODE_REALNUMBER);
			temp->real_value = $1;
			$$ = newNode(NODE_FACTOR);
			addChild($$, temp);
			printf("Reduction ( factor -> REALNUMBER)\n");
		}
	|
		SCIENTIFIC_EXP
		{
			NODE* temp = newNode(NODE_SCIENTIFIC_EXP);
			temp->real_value = $1;
			$$ = newNode(NODE_FACTOR);
			addChild($$, temp);
			printf("Reduction ( factor -> SCIENTIFIC_EXP)\n");
		}
	|
		addop REALNUMBER
		{
			$$ = newNode(NODE_FACTOR);
			NODE* temp = newNode(NODE_REALNUMBER);
			temp->real_value = $2;
			addChild($$, $1);
			addChild($$, temp);
			printf("Reduction ( factor -> addop REALNUMBER)\n");
		}
	|
		NUMBER
		{
			$$ = newNode(NODE_FACTOR);
			NODE* temp = newNode(NODE_NUMBER);
			temp->string = $1;
			addChild($$, temp);
			printf("Reduction ( factor -> NUMBER(STRING))\n");
		}
	|
		LPAREN expression RPAREN
		{
			$$ = newNode(NODE_FACTOR);
			addChild($$, newNode(NODE_LPAREN));
			addChild($$, $2);
			addChild($$, newNode(NODE_RPAREN));
			printf("Reduction ( factor -> LPAREN expression RPAREN)\n");
		}
	|
		NOT factor
		{
			$$ = newNode(NODE_FACTOR);
			addChild($$, newNode(NODE_NOT));
			addChild($$, $2);
			printf("Reduction ( factor -> not factor)\n");
		};

addop :
	PLUS
	{
		$$ = newNode(NODE_ADDOP);
		addChild($$, newNode(OP_ADD));
		printf("Reduction ( addop -> PLUS)\n");
	}
	|
		MINUS
		{
			$$ = newNode(NODE_ADDOP);
			addChild($$, newNode(OP_SUB));
			printf("Reduction ( addop -> MINUS)\n");
		};

mulop :
	STAR
	{
		$$ = newNode(NODE_MULOP);
		addChild($$, newNode(OP_MUL));
		printf("Reduction ( mulop -> STAR)\n");
	}
	|
		SLASH
		{
			$$ = newNode(NODE_MULOP);
			addChild($$, newNode(OP_DIV));
			printf("Reduction ( mulop -> SLASH)\n");
		};

relaop :
	LT
	{
		$$ = newNode(NODE_RELAOP);
		addChild($$, newNode(OP_LT));
		printf("Reduction ( relaop -> LT)\n");
	}
	|
		GT
		{
			$$ = newNode(NODE_RELAOP);
			addChild($$, newNode(OP_GT));
			printf("Reduction ( relaop -> GT)\n");
		}
	|
		EQUAL
		{
			$$ = newNode(NODE_RELAOP);
			addChild($$, newNode(OP_EQUAL));
			printf("Reduction ( relaop -> EQUAL)\n");
		}
	|
		LE
		{
			$$ = newNode(NODE_RELAOP);
			addChild($$, newNode(OP_LE));
			printf("Reduction ( relaop -> LE)\n");
		}
	|
		GE
		{
			$$ = newNode(NODE_RELAOP);
			addChild($$, newNode(OP_GE));
			printf("Reduction ( relaop -> GE)\n");
		}
	|
		NOTEQUAL
		{
			$$ = newNode(NODE_RELAOP);
			addChild($$, newNode(OP_NOTEQUAL));
			printf("Reduction ( relaop -> NOTEQUAL)\n");
		};


identifier_list :
	IDENTIFIER
	{
		NODE* temp = newNode(NODE_IDENTIFIER);
		temp->id = $1;
		$$ = newNode(NODE_IDENTIFIER_LIST);
		addChild($$, temp);
		printf("Reduction ( identifier_list -> IDENTIFIER)\n");
	}
	|
		identifier_list COMMA IDENTIFIER
		{
			$$ = newNode(NODE_IDENTIFIER_LIST);
			addChild($$, $1);
			addChild($$, newNode(NODE_COMMA));
			NODE* temp = newNode(NODE_IDENTIFIER);
			temp->id = $3;
			addChild($$, temp);
			printf("Reduction ( identifier_list COMMA IDENTIFIER)\n");
		};
lambda : ;

%%

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

int main(int argc, char** argv)
{
	int parse_result;
	char* filename = argv[1];
	FILE* file;
	file = fopen(filename,"r");
	if (!file)
		exit(1);
	extern FILE* yyin;
	yyin = file;
	parse_result = yyparse();

	fclose(file);
	if(parse_result==0)
	{
		printf("Parsing correctly\n");
		printf("*********************************************\n");
		printTree(ASTROOT,0);
	}
	else
	{
		printf("Parsing error\n");
	}
	return 0;
}
