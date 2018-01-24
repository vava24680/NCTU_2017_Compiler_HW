#ifndef CODE_GEN_H_
#define CODE_GEN_H_
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <map>
#include "node.h"
#include "symbol_table.h"
using namespace std;

class CODEGEN
{
public:
	CODEGEN();
	CODEGEN(string program_name);
	void SetProgramName(string program_name);
	void SetProgramName(char* program_name);
	void PrintInstruction(string instrunction);
	void push_front_scope_stack_codegen_used(int scope);
	void pop_front_scope_stack_codegen_used(void);
	void total_scope_increment_one(void);
	void Open_J_File(void);
	void PrintCommonPart(void);
	void Close_J_File(void);
public:
	string getProgramName(void) const;
	int get_total_scope(void) const;
	int get_current_scope(void) const;
	int get_scope_stack_size(void) const;
	bool search_from_symbol_table(string id, SymbolTable* symbol_table_ptr) const;
	vector<ENTRY>::const_iterator get_existing_entry_from_symtab(Node* node, SymbolTable* symbol_table_ptr) const;
	void PrintStackSize(int size);
	void PrintLocalSize(int size);
private:
	fstream fin;
	string program_name;
	deque<int> scope_stack_codegen_used;
	int total_scope;
};
string toLowerCase(string target);
template<class T> string numeric2string(T value);
void ClearTraversed(Node* node_start);
void CodeGen_Traversal(Node* node_start, CODEGEN* codegen_ptr);
void LHS_handler(Node* lhs_node, CODEGEN* codegen_ptr);
string FunctionProcedureParameter_Handler(Node* lparen_node);
int Predicate_Handler(Node* predicate_start_node);
void Operand_TypeCasting(Node* op1, CODEGEN* codegen_ptr);
#endif