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
	bool operator==(ParameterList &second);
	void add_in_one_type(string &type);
	void PrintAll(void) const;
private:
	vector<string> list;
};

typedef struct symbol_table_entry
{
	string id;
	string type;
	int belong_scope;
	int dimension;
	ParameterList parameter_list;
	Node* array_type;
}ENTRY;

class SymbolTable
{
public:
	SymbolTable();
public:
	void new_scope(void);
	bool add_in_one_entry(string id, string type, int belong_scope, int dimension, ParameterList parameter_list, Node* array_type);
	bool search_entry(string id);
	bool search_entry(string id, int current_scope);
	void printSymbolTable(void) const;
	string get_entry_DataType(Node* node);
	vector<ENTRY>::const_iterator get_one_entry(Node* node) const;
public:
	int get_total_scope(void) const;
private:
	vector< vector<ENTRY> > symbol_list;
	vector< set<string> > search_list;
};
bool DataTypeChecking(Node* first_node, Node* second_node);
string get_nodeType_name(int nodeType_number);
void TraversalParameter(Node* start_node, ParameterList &parameter_list);
string get_array_basic_DataType(Node* array_type_head);
void traversal(Node* node_start);
#endif