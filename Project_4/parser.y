
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
#include "symbol_table.h"
#include "CodeGen.h"
#include <cstring>
using namespace std;
extern "C"
{
	extern int yylex(void);
	int yyparse(void);
	void yyerror(const char *str)
	{
		fprintf(stderr,"error: %s\n",str);
	}
}

Node* ASTROOT;
CODEGEN codegen_instance;
int serial_number = 0;
map<string, string> FunctionNameList;
map<string, string> ProcedureNameList;
typedef map<string, int>::iterator MAPITERATOR;

%}

%union sementic{
	int integer_value;
	double real_value;
	Node* node;
	struct xxxx{
		char* id;
		int line_no;
	} id_struct;
}


%start prog

%token ARRAY
%token DO
%token ELSE
%token END
%token FUNCTION
%token IF
%token NOT
%token OF
%token PBEGIN
%token PROCEDURE
%token PROGRAM
%token THEN
%token VAR
%token WHILE
%token <id_struct> IDENTIFIER
%token ASSIGNMENT
%token COLON
%token COMMA
%token <integer_value> INTEGER
%token  DOT
%token  DOTDOT
%token  EQUAL
%token  GE
%token  GT
%token  LBRAC
%token  LE
%token  LPAREN
%token  LT
%token  MINUS
%token  PLUS
%token  RBRAC
%token <real_value> REALNUMBER
%token  RPAREN
%token  SEMICOLON
%token  SLASH
%token  STAR
%token  NOTEQUAL
%token <real_value> SCIENTIFIC_EXP
%token  SCIENTIFIC_EXP_EXPON
%token <id_struct> STRING
%token  INT_WORD
%token  REAL_WORD
%token  STRING_WORD
%token	PRINT
%token	WRITE

%type <node> prog
%type  <node> identifier_list
%type  <node> declarations
%type  <node> type
%type  <node> standard_type
%type  <node> subprogram_declarations
%type  <node> subprogram_declaration
%type  <node> subprogram_head
%type  <node> arguments
%type  <node> parameter_list
%type  <node> optional_var
%type  <node> compound_statement
%type  <node> optional_statements
%type  <node> statement_list
%type  <node> statement
%type  <node> variable
%type  <node> tail
%type  <node> procedure_statement
%type  <node> expression_list
%type  <node> expression
%type  <node> simple_expression
%type  <node> term
%type  <node> factor
%type  <node> addop
%type  <node> mulop
%type  <node> relaop
%type  <node> lambda
%left PLUS MINUS
%left STAR SLASH
%%

prog :
	PROGRAM IDENTIFIER LPAREN identifier_list RPAREN SEMICOLON
	declarations
	subprogram_declarations
	compound_statement DOT
	{
		Node* block_node = newNode(NODE_BLOCK);
		Node* identifier_node = newNode(NODE_IDENTIFIER);
		Node* program_node = newNode(NODE_PROGRAM);
		string temp($2.id);
		identifier_node->set_id(temp);
		identifier_node->set_line_no($2.line_no);
		block_node->modify_block_type(GLOBAL_BLOCK);
		$9->modify_block_type(PROGRAM_BODY_BLOCK);

		AdoptChild(identifier_node, $4);
		AdoptChild(program_node, identifier_node);
		MakeSiblings(program_node, $7);
		MakeSiblings(program_node, $8);
		MakeSiblings(program_node, $9);
		AdoptChild(block_node, program_node);
		$$ = block_node;
		ASTROOT = $$;
		//printf("Reduction ( prog -> program identifier LPAREN identifier_list RPAREN SEMICOLON declarations subprogram_declarations compound_statement DOT)\n");
	}
	;

declarations :
	declarations VAR identifier_list COLON type SEMICOLON
	{
		Node* var_node = newNode(NODE_VAR);
		AdoptChild($5, $3);
		AdoptChild(var_node, $5);
		MakeSiblings($1, var_node);
		$$ = $1;
		//printf("Reduction ( declarations -> declarations VAR identifier_list COLON type SEMICOLON)\n");
	}
	|
		lambda
		{
			$$ = $1;
			//printf("Reduction ( declarations -> lambda\n");
		};

type :
	standard_type
	{
		/*Single*/
		$$ = $1;
		//printf("Reduction ( type -> standard type)\n");
	}
	|	ARRAY LBRAC INTEGER DOTDOT INTEGER RBRAC OF type
		{//Special, use child
			Node* array_node = newNode(NODE_ARRAY);
			array_node->set_array_type($8);
			array_node->set_array_start_index($3);
			array_node->set_array_end_index($5);
			$$ = array_node;
			//printf("Reduction ( type -> ARRAY LBRAC INTEGER DOTDOT INTEGER RBRAC OF type)\n");
		};

standard_type :
	INT_WORD
	{
		/*Single*/
		$$ = newNode(NODE_INT_WORD);
		//printf("Reduction ( standard_type -> INT_WORD)\n");
	}
	|
		REAL_WORD
		{
			/*Single*/
			$$ = newNode(NODE_REAL_WORD);
			//printf("Reduction ( standard_type -> REAL_WORD)\n");
		}
	|
		STRING_WORD
		{
			/*Single*/
			$$ = newNode(NODE_STRING_WORD);
			//printf("Reduction ( standard_type -> STRING_WORD)\n");
		};

subprogram_declarations :
	subprogram_declarations subprogram_declaration SEMICOLON
	{
		MakeSiblings($1, $2);
		$$ = $1;
		//printf("Reduction ( subprogram_declarations -> subprogram_declarations subprogram_declaration SEMICOLON)\n");
	}
	|
		lambda
		{
			$$ = $1;
			//printf("Reduction ( subprogram_declarations -> lambda)\n");
		};

subprogram_declaration :
	subprogram_head declarations compound_statement
	{//A block
		AdoptChild($1->get_leftmost_child()->get_rsibling(), $2);
		AdoptChild($1->get_leftmost_child()->get_rsibling(), $3);
		$$ = $1;
		int i = 0;
		Node* var_node = $2->get_rsibling();
		Node* type_node = NULL;
		Node* id_node = NULL;
		for(; var_node != NULL; var_node = var_node->get_rsibling())
		{
			type_node = var_node->get_leftmost_child();
			id_node = type_node->get_leftmost_child();
			while(id_node != NULL)
			{
				id_node->set_local_serial_number(i);
				id_node = id_node->get_rsibling();
				i++;
			}
		}
		//printf("Reduction ( subprogram_declaration -> subprogram_head declarations compound_statement)\n");
	};

subprogram_head :
	FUNCTION IDENTIFIER arguments COLON standard_type SEMICOLON
	{
		Node* function_node = newNode(NODE_FUNCTION);
		Node* identifier_node = newNode(NODE_IDENTIFIER);
		Node* block_node = newNode(NODE_BLOCK);

		string temp($2.id);
		identifier_node->set_id(temp);
		identifier_node->set_line_no($2.line_no);
		AdoptChild($5, identifier_node);
		AdoptChild(block_node, $3);

		AdoptChild(function_node, $5);
		AdoptChild(function_node, block_node);
		$$ = function_node;
		//printf("Reduction ( subprogram_head -> FUNCTION identifier arguments COLON standard_type SEMICOLON)\n");
	}
	|
		PROCEDURE IDENTIFIER arguments SEMICOLON
		{
			Node* identifier_node = newNode(NODE_IDENTIFIER);
			Node* procedure_node = newNode(NODE_PROCEDURE);
			Node* block_node = newNode(NODE_BLOCK);
			string temp($2.id);
			identifier_node->set_id(temp);
			identifier_node->set_line_no($2.line_no);
			identifier_node->set_data_type("PROCEDURE");
			AdoptChild(block_node, $3);
			AdoptChild(procedure_node, identifier_node);
			AdoptChild(procedure_node, block_node);
			$$ = procedure_node;
			//printf("Reduction ( subprogram_head -> PROCEDURE identifier arguments SEMICOLON)\n");
		};

arguments :
	LPAREN parameter_list RPAREN
	{
		Node* lparen_node = newNode(NODE_LPAREN);
		AdoptChild(lparen_node, $2);
		$$ = lparen_node;
		//printf("Reduction ( arguments -> LPAREN parameter_list RPAREN)\n");
	}
	|
		lambda
		{
			$$ = $1;
			//printf("Reduction ( arguments -> lambda)\n");
		};

parameter_list :
	optional_var identifier_list COLON type
	{
		AdoptChild($4, $2);
		AdoptChild($1, $4);
		$$ = $1;
		int i = 0;
		for(Node* ptr = $2; ptr != NULL; ptr = ptr->get_rsibling())
		{
			ptr->set_local_serial_number(i);
			i++;
		}
		printf("Reduction ( parameter_list -> optional_var identifier_list COLON type)\n");
	}
	|
		optional_var identifier_list COLON type SEMICOLON parameter_list
		{
			AdoptChild($4, $2);
			AdoptChild($1, $4);
			MakeSiblings($1, $6);
			$$ = $1;
			int i = 0;
			for(Node* ptr = $2; ptr != NULL; ptr = ptr->get_rsibling())
			{
				ptr->set_local_serial_number(i);
				i++;
			}
			//printf("Reduction ( parameter_list -> optional_var identifier_list COLON type SEMICOLON parameter_list)\n");
		};

optional_var :
	VAR
	{
		/*Single*/
		$$ = newNode(NODE_VAR);
		//printf("Reduction ( optional_var -> VAR)\n");
	}
	|
		lambda
		{
			$$ = $1;
			//printf("Reduction ( optional_var -> lambda)\n");
		};

compound_statement :
	PBEGIN optional_statements END
	{
		Node* block_node = newNode(NODE_BLOCK);
		block_node->modify_block_type(COMPOUND_BLOCK);
		AdoptChild(block_node, $2);
		$$ = block_node;
		//printf("Reduction ( compound_statement -> begin optional_statements end)\n");
	};

optional_statements :
	statement_list
	{
		/*Single*/
		$$ = $1;
		//printf("Reduction ( optional_statements -> statement_list)\n");
	};

statement_list :
	statement
	{
		/*Single*/
		$$ = $1;
		//printf("Reduction ( statement_list -> statement)\n");
	}
	|
		statement_list SEMICOLON statement
		{
			MakeSiblings($1, $3);
			$$ = $1;
			//printf("Reduction ( statement_list -> statement_list SEMICOLON statement)\n");
		};

statement :
	variable ASSIGNMENT expression
	{//Special, use child
	/*Code-Gen Done*/
		Node* assignment_node = newNode(NODE_ASSIGNMENT);
		AdoptChild(assignment_node, $1);
		AdoptChild(assignment_node, $3);
		$$ = assignment_node;
		//printf("Reduction ( statement -> variable ASSIGNMENT expression)\n");
	}
	|
		procedure_statement
		{
		/*Code-Gen Done*/
			/*Single*/
			$$ = $1;
			//printf("Reduction ( statement -> procedure_statement)\n");
		}
	|
		compound_statement
		{
			/*Single*/
			$$ = $1;
			//printf("Reduction ( statement -> compound_statement)\n");
		}
	|
		IF expression THEN statement ELSE statement
		{
			Node* block_node = newNode(NODE_BLOCK);
			Node* block_node_2 = newNode(NODE_BLOCK);
			Node* if_node = newNode(NODE_IF);
			block_node->modify_block_type(THEN_BLOCK);
			block_node_2->modify_block_type(ELSE_BLOCK);
			AdoptChild(block_node, $4);
			AdoptChild(block_node_2, $6);
			AdoptChild(if_node, $2);
			AdoptChild(if_node, block_node);
			AdoptChild(if_node, block_node_2);
			$$ = if_node;
			//printf("Reduction ( statement -> IF expression THEN statement ELSE statement)\n");
		}
	|
		WHILE expression DO statement
		{
			Node* block_node = newNode(NODE_BLOCK);
			Node* while_node = newNode(NODE_WHILE);
			block_node->modify_block_type(WHILE_BLOCK);
			AdoptChild(while_node, $2);
			AdoptChild(block_node, $4);
			AdoptChild(while_node, block_node);
			$$ = while_node;
			//printf("Reduction ( statement -> WHILE expression DO statement)\n");
		}
	|
		PRINT expression
		{
			Node* print_node = newNode(NODE_PRINT);
			AdoptChild(print_node, $2);
			$$ = print_node;
		}
	|
		WRITE expression
		{
			Node* write_node = newNode(NODE_WRITE);
			AdoptChild(write_node, $2);
			$$ = write_node;
		}
	|

		lambda
		{
			$$ = $1;
			//printf("Reduction ( statement -> lambda)\n");
		};

variable :
	IDENTIFIER tail
	{
		Node* identifier_node = newNode(NODE_IDENTIFIER);
		string temp($1.id);
		identifier_node->set_id(temp);
		identifier_node->set_line_no($1.line_no);
		AdoptChild(identifier_node, $2);
		$$ = identifier_node;
		//printf("Reduction ( variable -> identifier tail)\n");
	};

tail :
	LBRAC expression RBRAC tail
	{
		MakeSiblings($2, $4);
		$$ = $2;
		//printf("Reduction ( tail -> LBRAC expression RBRAC tail)\n");
	}
	|
		lambda
		{
			$$ = $1;
			//printf("Reduction ( tail -> lambda)\n");
		};

procedure_statement :
	IDENTIFIER
	{
		Node* identifier_node = newNode(NODE_IDENTIFIER);
		identifier_node->set_id($1.id);
		identifier_node->set_line_no($1.line_no);
		$$ = identifier_node;
		//printf("Reduction ( procedure_statement -> identifier)\n");
	}
	|
		IDENTIFIER LPAREN expression_list RPAREN
		{
			Node* identifier_node = newNode(NODE_IDENTIFIER);
			Node* lparen_node = newNode(NODE_LPAREN);
			string temp($1.id);
			identifier_node->set_id(temp);
			identifier_node->set_line_no($1.line_no);
			AdoptChild(lparen_node, $3);
			AdoptChild(identifier_node, lparen_node);
			$$ = identifier_node;
			printf("Reduction ( procedure_statement -> identifier LPAREN expression_list RPAREN)\n");
		};

expression_list :
	expression
	{
		/*Single*/
		$$ = $1;
		//printf("Reduction ( expression_list -> expression)\n");
	}
	|
		expression_list COMMA expression
		{
			MakeSiblings($1, $3);
			$$ = $1;
			//printf("Reduction ( expression_list -> expression_list COMMA expression)\n");
		};

expression :
	simple_expression
	{
		/*Single*/
		$$ = $1;
		//printf("Reduction ( expression -> simple_expression)\n");
	}
	|
		simple_expression relaop simple_expression
		{//Special, using child
			AdoptChild($2, $1);
			AdoptChild($2, $3);
			$$ = $2;
			//printf("Reduction ( expression -> simple_expression relaop simple_expression)\n");
		};

simple_expression :
	term
	{
		/*Single*/
		$$ = $1;
		//printf("Reduction ( simple_expression -> term)\n");
	}
	|
		simple_expression addop term
		{//Special, using child
			AdoptChild($2, $1);
			AdoptChild($2, $3);
			$$ = $2;
			/*string type = $1->get_data_type()=="INTEGER" ? "i" : "f";
			switch($2->get_node_type())
			{
				case OP_ADD:
					codegen_instance.PrintInstruction(type + "add");
					break;
				case OP_SUB:
					codegen_instance.PrintInstruction(type + "sub");
					break;
			}*/
			//printf("Reduction ( simple_expression -> simple_expression addop term)\n");
		};

term :
	factor
 	{
		/*Single*/
		$$ = $1;
		//printf("Reduction ( term -> factor)\n");
	}
	|
		term mulop factor
		{//Special, using child
			AdoptChild($2, $1);
			AdoptChild($2, $3);
			$$ = $2;
			/*string type = $1->get_data_type()=="INTEGER" ? "i" : "f";
			switch($2->get_node_type())
			{
				case OP_MUL:
					codegen_instance.PrintInstruction(type + "mul");
					break;
				case OP_SUB:
					codegen_instance.PrintInstruction(type + "div");
					break;
			}*/
			//printf("Reduction ( term -> term mulop factor)\n");
		};

factor :
	IDENTIFIER tail
	{
		Node* identifier_node = newNode(NODE_IDENTIFIER);
		string temp($1.id);
		identifier_node->set_id(temp);
		identifier_node->set_line_no($1.line_no);
		AdoptChild(identifier_node, $2);
		$$ = identifier_node;
		//printf("Reduction ( factor -> identifier tail)\n");
	}
	|
		IDENTIFIER LPAREN expression_list RPAREN
		{
			Node* identifier_node = newNode(NODE_IDENTIFIER);
			Node* lparen_node = newNode(NODE_LPAREN);
			string temp($1.id);
			identifier_node->set_id(temp);
			identifier_node->set_line_no($1.line_no);
			AdoptChild(lparen_node, $3);
			AdoptChild(identifier_node, lparen_node);
			$$ = identifier_node;
			//printf("Reduction ( factor -> identifier LPAREN expression_list RPAREN)\n");
		}
	|
		INTEGER
		{
			Node* integer_node = newNode(NODE_INTEGER);
			integer_node->set_integer_value($1);
			integer_node->set_data_type("INTEGER");
			$$ = integer_node;
			//printf("Reduction ( factor -> INTEGER)\n");
		}
	|
		addop INTEGER
		{
			Node* integer_node = newNode(NODE_INTEGER);
			integer_node->set_integer_value($2);
			integer_node->set_data_type("INTEGER");
			AdoptChild($1, integer_node);
			$$ = $1;
			//printf("Reduction ( factor -> addop INTEGER)\n");
		}
	|
		REALNUMBER
		{
			Node* realnumber_node = newNode(NODE_REALNUMBER);
			realnumber_node->set_real_value($1);
			realnumber_node->set_data_type("REAL");
			$$ = realnumber_node;
			//printf("Reduction ( factor -> REALNUMBER)\n");
		}
	|
		SCIENTIFIC_EXP
		{
			Node* sci_node = newNode(NODE_SCIENTIFIC_EXP);
			sci_node->set_real_value($1);
			sci_node->set_data_type("REAL");
			$$ = sci_node;
			//printf("Reduction ( factor -> SCIENTIFIC_EXP)\n");
		}
	|
		addop REALNUMBER
		{
			Node* realnumber_node = newNode(NODE_REALNUMBER);
			realnumber_node->set_real_value($2);
			realnumber_node->set_data_type("REAL");
			AdoptChild($1, realnumber_node);
			$$ = $1;
			//printf("Reduction ( factor -> addop REALNUMBER)\n");
		}
	|
		STRING
		{
			Node* number_node = newNode(NODE_NUMBER);
			string temp($1.id);
			number_node->set_id(temp);
			number_node->set_line_no($1.line_no);
			number_node->set_data_type("STRING");
			$$ = number_node;
			//printf("Reduction ( factor -> STRING(STRING))\n");
		}
	|
		LPAREN expression RPAREN
		{
			$$ = $2;
			//printf("Reduction ( factor -> LPAREN expression RPAREN)\n");
		}
	|
		NOT factor
		{
			Node* not_node = newNode(NODE_NOT);
			MakeSiblings(not_node, $2);
			$$ = not_node;
			//printf("Reduction ( factor -> not factor)\n");
		};

addop :
	PLUS
	{
		$$ = newNode(OP_ADD);
		//printf("Reduction ( addop -> PLUS)\n");
	}
	|
		MINUS
		{
			$$ = newNode(OP_SUB);
			//printf("Reduction ( addop -> MINUS)\n");
		};

mulop :
	STAR
	{
		$$ = newNode(OP_MUL);
		//printf("Reduction ( mulop -> STAR)\n");
	}
	|
		SLASH
		{
			$$ = newNode(OP_DIV);
			//printf("Reduction ( mulop -> SLASH)\n");
		};

relaop :
	LT
	{
		$$ = newNode(OP_LT);
		//printf("Reduction ( relaop -> LT)\n");
	}
	|
		GT
		{
			$$ = newNode(OP_GT);
			//printf("Reduction ( relaop -> GT)\n");
		}
	|
		EQUAL
		{
			$$ = newNode(OP_EQUAL);
			//printf("Reduction ( relaop -> EQUAL)\n");
		}
	|
		LE
		{
			$$ = newNode(OP_LE);
			//printf("Reduction ( relaop -> LE)\n");
		}
	|
		GE
		{
			$$ = newNode(OP_GE);
			//printf("Reduction ( relaop -> GE)\n");
		}
	|
		NOTEQUAL
		{
			$$ = newNode(OP_NOTEQUAL);
			//printf("Reduction ( relaop -> NOTEQUAL)\n");
		};


identifier_list :
	IDENTIFIER
	{
		Node* identifier_node = newNode(NODE_IDENTIFIER);
		string temp($1.id);
		identifier_node->set_id(temp);
		identifier_node->set_line_no($1.line_no);
		$$ = identifier_node;
		//printf("Reduction ( identifier_list -> IDENTIFIER)\n");
	}
	|
		identifier_list COMMA IDENTIFIER
		{
			Node* identifier_node = newNode(NODE_IDENTIFIER);
			string temp($3.id);
			identifier_node->set_id(temp);
			identifier_node->set_line_no($3.line_no);
			MakeSiblings($1, identifier_node);
			$$ = $1;
			//printf("Reduction ( identifier_list COMMA IDENTIFIER)\n");
		};
lambda :
{
	$$ = newNode(NODE_LAMBDA);
};

%%

SymbolTable symbol_table_instance;
deque<int> scope_stack;
deque< deque<string> > if_while_instruction_stack;
int scope_codegen_used = 0;

int main(int argc, char** argv)
{
	int parse_result, last_slash_pos, last_dot_pos;
	char* filename = argv[1];
	FILE* file;
	file = fopen(filename,"r");
	if (!file)
		exit(1);
	string temp(argv[1]);
	last_slash_pos = temp.find_last_of("/");
	last_dot_pos = temp.find_last_of(".");
	codegen_instance.SetProgramName(temp.substr(0, last_dot_pos));
	codegen_instance.Open_J_File();
	codegen_instance.SetProgramName(temp.substr(last_slash_pos + 1, last_dot_pos - last_slash_pos - 1));
	codegen_instance.PrintCommonPart();
	extern FILE* yyin;
	yyin = file;
	parse_result = yyparse();
	fclose(file);

	if(parse_result==0)
	{
		printf("Parsing correctly\n");
		printf("*********************************************\n");
		printTree(ASTROOT,0);
		traversal(ASTROOT);
		symbol_table_instance.printSymbolTable();
		/* 0,   Inititalize a CODEGEN Object*/
		//ClearTraversed(ASTROOT);
		cout << "Finish printSymbolTable" << endl;
		ClearTraversed(ASTROOT);
		CodeGen_Traversal(ASTROOT, &codegen_instance);
		codegen_instance.PrintInstruction("return");
		codegen_instance.PrintInstruction(".end method");
	}
	else
		printf("Parsing error\n");
	codegen_instance.Close_J_File();
	return 0;
}
