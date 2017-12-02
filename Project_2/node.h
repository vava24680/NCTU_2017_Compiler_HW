#ifndef __NODE_H__
#define __NODE_H__

//struct nodeType;
enum ntype {
	OP_ADD,
	OP_SUB,
	OP_MUL,
 	OP_DIV,
	OP_LT,
	OP_GT,
	OP_EQUAL,
	OP_LE,
	OP_GE,
	OP_NOTEQUAL,
	NODE_ARRAY,
	NODE_DO,
	NODE_ELSE,
	NODE_END,
	NODE_FUNCTION,
	NODE_IF,
	NODE_NOT,
	NODE_OF,
	NODE_PBEGIN,
	NODE_PROCEDURE,
	NODE_PROGRAM,
	NODE_THEN,
	NODE_VAR,
	NODE_WHILE,
	NODE_IDENTIFIER,
	NODE_ASSIGNMENT,
	NODE_COLON,
	NODE_COMMA,
	NODE_INTEGER,
	NODE_DOT,
	NODE_DOTDOT,
	NODE_LBRAC,
	NODE_LPAREN,
	NODE_RBRAC,
	NODE_REALNUMBER,
	NODE_RPAREN,
	NODE_SEMICOLON,
	NODE_NUMBER,
	NODE_INT_WORD,
	NODE_REAL_WORD,
	NODE_SCIENTIFIC_EXP,
	NODE_SCIENTIFIC_EXP_EXPON,
	NODE_STRING_WORD,
	NODE_PROG,
	NODE_DECLARATIONS,
	NODE_TYPE,
	NODE_STANDARD_TYPE,
	NODE_SUBDECLARATIONS,
	NODE_SUBDECLARATION,
	NODE_SUBPROGRAM_HEAD,
	NODE_ARGUMENTS,
	NODE_PARAM_LIST,
	NODE_OPTIONAL_VAR,
	NODE_COMP_STATE,
	NODE_OPTIONAL_STATE,
	NODE_STATEMENT_LIST,
	NODE_STATEMENT,
	NODE_VARIABLE,
	NODE_TAIL,
	NODE_PROC_STATE,
	NODE_EXPR_LIST,
	NODE_EXPR,
	NODE_SIM_EXPR,
	NODE_TERM,
	NODE_FACTOR,
	NODE_ADDOP,
	NODE_MULOP,
	NODE_RELAOP,
	NODE_IDENTIFIER_LIST,
	NODE_LAMBDA
};

// #include "symtab.h"
typedef struct nodeType {
	int nodeType;
	struct nodeType *parent;
	struct nodeType *child;
	struct nodeType *lsibling;
	struct nodeType *rsibling;

	/* Attribute for NODE_TOKEN */
	int tokenType;

	/* items for Array */
	int start_index;
	int end_index;
	int arraydepth;
	struct nodeType *ref;

	/* Values for general use */
	int integer_value;
	double real_value;
	char valueValid;
	char *string;
	char *id;

	/* Indicates which OP */
	char operator;

} NODE;

//NODE** store_into_array(NODE** arr, NODE* node,int* counter,int* size);
NODE* newNode(int type);
void deleteNode(NODE* node);
void addChild(NODE* node,NODE* child);
void printTree(NODE* node, int ident);
#endif

