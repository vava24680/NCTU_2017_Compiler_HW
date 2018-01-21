#include <string>
#include <iostream>
#include <vector>
#ifndef __NODE_H__
#define __NODE_H__
using namespace std;
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
	NODE_LAMBDA,
	NODE_BLOCK,
	NODE_ARRAY_UPPER_BOUND,
	NODE_ARRAY_LOWER_BOUND,
	NODE_PRINT,
	NODE_WRITE
};
typedef enum b_type
{
	NotBlock,
	GLOBAL_BLOCK,
	FUN_PROG_BLOCK,
	COMPOUND_BLOCK,
	THEN_BLOCK,
	ELSE_BLOCK,
	WHILE_BLOCK
} BlockType;
class Node
{
public:
	Node();
	Node(int node_type);
	void modify_block_type(BlockType block_type);
	void set_id(const string id);
	void set_integer_value(const int integer_value);
	void set_real_value(const double real_value);
	void set_array_start_index(const int array_start_index);
	void set_array_end_index(const int array_end_index);
	void set_line_no(const int line_no);
	void set_is_traversed(void);
	void set_data_type(string data_type);
	void set_array_type(Node* array_type);
	void set_local_serial_number(int local_serial_number);
	void modify_leftmost_child(Node* leftmost_child);
	void modify_parent(Node* parent);
	void modify_leftmost_sibling(Node* leftmost_sibling);
	void modify_rsibling(Node* rsibling);
	void clear_traversed(void);
public:
	string get_id(void) const;
	int get_integer_value(void) const;
	double get_real_value(void) const;
	int get_array_start_index(void) const;
	int get_array_end_index(void) const;
	bool get_is_traversed(void) const;
	int get_block_type(void) const;
	int get_node_type(void) const;
	int get_line_no(void) const;
	int get_local_serial_number(void) const;
	string get_data_type(void) const;
	Node* get_node_array_type(void) const;
	Node* get_parent(void) const;
	Node* get_leftmost_child(void) const;
	Node* get_leftmost_sibling(void) const;
	Node* get_rsibling(void) const;
private:
	/*Internal Attribute*/
	BlockType block_type;
	string id;
	int integer_value;
	int real_value;
	int array_start_index;
	int array_end_index;
	bool is_traversed;
	int line_no;
	Node* array_type;
	string data_type;
	int local_serial_number;
private:
	/*Default Attributes*/
	int node_type;
	Node* parent;
	Node* leftmost_child;
	Node* leftmost_sibling;
	Node* rsibling;
};

Node* newNode(int type);
//void deleteNode(NODE* node);
void MakeSiblings(Node* master, Node* node_add_in);
void AdoptChild(Node* master, Node*child_node_add_in);
void printTree(Node* node, int ident);
#endif