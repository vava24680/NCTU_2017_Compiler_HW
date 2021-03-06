#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
#include <utility>
#include <vector>
#include <map>
#include <cstdio>
#include <string>
#include <climits>
#include <deque>
#include <set>
#include <iomanip>
#include "node.h"
using namespace std;
typedef map<string, int>::iterator MAPITERATOR;
class SymbolTable;
class ParameterList
{
public:
	//ParameterList();
	bool operator==(const ParameterList &second) const;
	void add_in_one_type(string type);
	unsigned int get_numbers_parameter(void) const;
	void PrintAll(void) const;
private:
	vector<string> list;
};

typedef struct symbol_table_entry
{
	string id;
	string type;
	string return_type;
	int belong_scope;
	int dimension;
	ParameterList parameter_list;
	Node* array_type;
	int local_serial_number;
}ENTRY;

class SymbolTable
{
public:
	SymbolTable();
public:
	void new_scope(void);
	bool add_in_one_entry(string id, string type, string return_type, int belong_scope, int dimension, ParameterList parameter_list, Node* array_type, int local_serial_number);
	bool search_entry(string id) const;
	bool search_entry(string id, const deque<int>& scope_stack) const;
	void printSymbolTable(void) const;
	string get_ExistingEntry_DataType(Node* node) const;
	vector<ENTRY>::const_iterator get_existing_entry(Node* node) const;
	vector<ENTRY>::const_iterator get_existing_entry(Node* node, const deque<int>& scope_stack) const;
public:
	int get_total_scope(void) const;
private:
	vector< vector<ENTRY> > symbol_list;
	vector< set<string> > search_list;
};
bool DataTypeChecking(Node* first_node, Node* second_node);
string get_nodeType_name(int nodeType_number);
void TraverseFunctionDeclareParameter(Node* start_node, ParameterList &parameter_list);
void TraverseFunctionCallParameter(Node* start_node, ParameterList &parameter_list);
string get_array_basic_DataType(Node* array_type_head);
unsigned int calculate_array_current_dimension(Node* node);
bool array_current_dimesion_check(Node* node);
bool parse_identifier_node_in_expression(Node* identifier_node);
void traversal(Node* node_start);
#endif