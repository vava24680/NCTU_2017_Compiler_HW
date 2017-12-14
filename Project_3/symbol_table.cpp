#include "symbol_table.h"
using namespace std;
static deque<SymbolTable*> symbol_table_stack;
extern SymbolTable symbol_table_instance;
extern deque<int> scope_stack;
//Const Reference since the Parameter delivered in is a intermidiate value
bool ParameterList::operator==(const ParameterList &second) const
{
	unsigned int list_size = this->list.size();
	if(this->list.size() != second.list.size())
	{
		return false;
	}
	for(unsigned int i =0; i < list_size ; i++)
	{
		if(this->list[i] != second.list[i])
		{
			return false;
		}
	}
	return true;
}
void ParameterList::add_in_one_type(string type)
{
	this->list.push_back(type);
}
unsigned int ParameterList::get_numbers_parameter(void) const
{
	return this->list.size();
}
void ParameterList::PrintAll(void) const
{
	for(unsigned int i = 0; i < this->list.size(); i++)
	{
		cout << this->list[i] << endl;
	}
}
SymbolTable::SymbolTable(){};
int SymbolTable::get_total_scope(void) const
{
	return this->symbol_list.size();
}
void SymbolTable::new_scope(void)
{
	this->symbol_list.push_back(vector<ENTRY>());
	this->search_list.push_back(set<string>());
}
bool SymbolTable::add_in_one_entry(string id, string type, string return_type, int belong_scope, int dimension, ParameterList parameter_list, Node* array_type)
{
	if(this->search_list[belong_scope].find(id) != this->search_list[belong_scope].end())
		return false;
	ENTRY temp;
	temp.id = id;
	temp.type = type;
	temp.return_type = type;
	temp.belong_scope = belong_scope;
	temp.dimension = dimension;
	temp.parameter_list = parameter_list;
	temp.array_type = array_type;
	(this->symbol_list[belong_scope]).push_back(temp);
	(this->search_list[belong_scope]).insert(id);
	return true;
}
bool SymbolTable::search_entry(string id) const
{
	for(int i = 0; i < scope_stack.size(); i++)
	{
		if( this->search_list[scope_stack[i]].find(id) != this->search_list[scope_stack[i]].end() )
			return true;
	}
	return false;
}

void SymbolTable::printSymbolTable(void) const
{
	unsigned int symbol_list_size = this->symbol_list.size();
	cout.setf(iostream::left);
	cout << setw(10) << "Name" << setw(10) << "Type" << setw(10) << "Scope" << setw(10) << "Dimension" << endl;
	for(unsigned int i = 0; i < symbol_list_size; i++)
	{
		for(unsigned int j = 0; j < this->symbol_list[i].size(); j++)
		{
			cout.setf(iostream::left);
			cout << setw(10) << this->symbol_list[i][j].id << setw(10) << this->symbol_list[i][j].type << setw(10) << this->symbol_list[i][j].belong_scope << setw(10) << this->symbol_list[i][j].dimension << endl;
		}
	}
}

string SymbolTable::get_ExistingEntry_DataType(Node* node) const
{
	for(unsigned int i = 0;i < scope_stack.size();i++)
	{
		for(unsigned int j = 0;j < this->symbol_list[scope_stack[i]].size(); j++)
		{
			if(node->get_id() == this->symbol_list[scope_stack[i]][j].id)
			{
				return this->symbol_list[scope_stack[i]][j].type;
			}
		}
	}
}

vector<ENTRY>::const_iterator SymbolTable::get_existing_entry(Node* node) const
{
	for(unsigned int i = 0;i < scope_stack.size();i++)
	{
		vector<ENTRY>::const_iterator it = this->symbol_list[scope_stack[i]].begin();
		for(; it != this->symbol_list[scope_stack[i]].end(); it++)
		{
			if(it->id == node->get_id())
			{
				return it;
			}
		}
	}
}
bool DataTypeChecking(Node* first_node, Node* second_node)
{
	//cout << "first_node " << first_node << endl;
	//cout << "second_node " << second_node << endl;
	string left_data_type, right_data_type;
	//cout << "DataTypeChecking " << endl;
	if(first_node->get_node_type() == NODE_ARRAY && second_node->get_node_type() == NODE_ARRAY)
	{//Array node type checking
		//cout << "Array" << endl;
		if(first_node->get_array_start_index() != second_node->get_array_start_index())
			return false;
		if(first_node->get_array_end_index() != second_node->get_array_end_index())
			return false;

		if(first_node->get_node_array_type()->get_node_type() != NODE_ARRAY || second_node->get_node_array_type()->get_node_type() != NODE_ARRAY)
		{
			if( first_node->get_node_array_type()->get_node_type() ==
				second_node->get_node_array_type()->get_node_type() )
				return true;
			else
				return false;
		}
		else
			return DataTypeChecking(first_node->get_node_array_type(), first_node->get_node_array_type());
	}
	else
	{
		if(symbol_table_instance.get_existing_entry(first_node)->type == "FUNCTION")
			left_data_type = symbol_table_instance.get_existing_entry(first_node)->return_type;
		else
			left_data_type = first_node->get_data_type();
		if(!symbol_table_instance.search_entry(second_node->get_id()))
			right_data_type = second_node->get_data_type();
		else
		{
			if(symbol_table_instance.get_existing_entry(second_node)->type == "FUNCTION")
				right_data_type = symbol_table_instance.get_existing_entry(second_node)->return_type;
			else
				right_data_type = second_node->get_data_type();
		}
		if(left_data_type == "INTEGER" && right_data_type == "REAL")
			return false;
		else
			return true;
	}
}
string get_nodeType_name(int nodeType_number)
{
	switch (nodeType_number) {
		case NODE_INT_WORD:
			return "INTEGER";
			break;
		case NODE_REAL_WORD:
			return "REAL";
			break;
		case NODE_STRING_WORD:
			return "STRING";
			break;
		case NODE_ARRAY:
			return "ARRAY";
			break;
		case NODE_PROCEDURE:
			return "PROCEDURE";
			break;
		case NODE_FUNCTION:
			return "FUNCTION";
			break;
		default:
			return "NOT";
			break;
	}
}
void TraverseFunctionDeclareParameter(Node* start_node, ParameterList &parameter_list)
{
	int node_type = start_node->get_node_type();
	string type_name = get_nodeType_name(node_type);
	if(type_name.compare("NOT") != 0)
	{
		Node* temp = start_node->get_leftmost_child();
		do
		{
			parameter_list.add_in_one_type(type_name);
			temp = temp->get_rsibling();
		} while(temp != NULL);
		return;
	}
	if(start_node->get_leftmost_child() != NULL)
		TraverseFunctionDeclareParameter(start_node->get_leftmost_child(), parameter_list);
	if(start_node->get_rsibling() != NULL)
		TraverseFunctionDeclareParameter(start_node->get_rsibling(), parameter_list);
	return;
}
void TraverseFunctionCallParameter(Node* start_node, ParameterList &parameter_list)
{
	while(start_node != NULL)
	{
		int node_type_number = start_node->get_node_type();
		switch (node_type_number)
		{
			case NODE_INT_WORD:
			case NODE_REAL_WORD:
			case NODE_STRING_WORD:
			{
				parameter_list.add_in_one_type(start_node->get_data_type());
				break;
			}
			case OP_SUB:
			case OP_ADD:
			{
				//parameter_list.add_in_one_type(start_node->get_leftmost_child()->get_data_type());
				if(start_node->get_leftmost_child() == NULL)
					cout << "[ERROR] Expression is not complete" << endl;
				else
				{
					if(start_node->get_leftmost_child()->get_node_type() == NODE_IDENTIFIER)
					{
						if(!symbol_table_instance.search_entry(start_node->get_leftmost_child()->get_id()))
							cout << "[ERROR] Undeclared Variable \"" << start_node->get_leftmost_child()->get_id()  << "\" at line " << start_node->get_leftmost_child()->get_line_no() << endl;
						else
						{
							string temp;
							if(symbol_table_instance.get_ExistingEntry_DataType(start_node->get_leftmost_child()) == "FUNCTION")
							{
								temp = symbol_table_instance.get_existing_entry(start_node->get_leftmost_child())->return_type;
							}
							else
								temp = symbol_table_instance.get_ExistingEntry_DataType(start_node->get_leftmost_child());
							parameter_list.add_in_one_type(temp);
						}
					}
					else
						parameter_list.add_in_one_type(start_node->get_leftmost_child()->get_data_type());
				}
				break;
			}
			case NODE_IDENTIFIER:
			{
				/*string temp(symbol_table_instance.get_ExistingEntry_DataType(start_node));
				parameter_list.add_in_one_type(temp);*/
				if(!symbol_table_instance.search_entry(start_node->get_id()))
					cout << "[ERROR] Undeclared Variable \"" << start_node->get_id()  << "\" at line " << start_node->get_line_no() << endl;
				else
				{
					string temp;
					if(symbol_table_instance.get_ExistingEntry_DataType(start_node) == "FUNCTION")
					{
						temp = symbol_table_instance.get_existing_entry(start_node)->return_type;
					}
					else
						temp = symbol_table_instance.get_ExistingEntry_DataType(start_node);
					parameter_list.add_in_one_type(temp);
				}
				break;
			}
			default:
			{
				parameter_list.add_in_one_type(start_node->get_data_type());
				break;
			}
		}
		start_node = start_node->get_rsibling();
	}
}
string get_array_basic_DataType(Node* array_type_head)
{
	Node* temp = array_type_head;
	while(temp->get_node_array_type() != NULL)
	{
		temp = temp->get_node_array_type();
	}
	return get_nodeType_name(temp->get_node_type());
}
unsigned int calculate_array_current_dimension(Node* node)
{
	Node* temp = node->get_leftmost_child();
	int current_dimension = 0;
	while(temp->get_node_type() != NODE_LAMBDA)
	{
		current_dimension++;
		temp = temp->get_rsibling();
	}
	return current_dimension;
}
bool array_current_dimesion_check(Node* node)
{
	const int dimension = symbol_table_instance.get_existing_entry(node)->dimension;
	if(calculate_array_current_dimension(node) == dimension)
		return true;
	else
		return false;
}
void traversal(Node* node)
{
	//cout << "Node type : " << node->get_node_type() << endl;
	if(node == NULL)
		return;
	if(!node->get_is_traversed())
	{
		switch (node->get_node_type())
		{
			case NODE_BLOCK:
			{
				symbol_table_instance.new_scope();
				scope_stack.push_front(symbol_table_instance.get_total_scope() - 1);
				break;
			}
			case NODE_PROGRAM:
			{
				Node* child = node->get_leftmost_child();
				symbol_table_instance.add_in_one_entry(child->get_id(), "INITIAL", "NONE", scope_stack.front(), 0, ParameterList(), NULL);
				child = child->get_leftmost_child();
				do
				{
					symbol_table_instance.add_in_one_entry(child->get_id(), "INITIAL", "NONE", scope_stack.front(), 0, ParameterList(), NULL);
					child = child->get_rsibling();
				} while(child != NULL);
				node->get_leftmost_child()->set_is_traversed();
				break;
			}
			case NODE_PROCEDURE:
			{
				ParameterList parameter_list;
				Node* lparen_node = node->get_leftmost_child()->get_rsibling()->get_leftmost_child();
				string return_type(get_nodeType_name(node->get_leftmost_child()->get_node_type()));
				TraverseFunctionDeclareParameter(lparen_node->get_leftmost_child(), parameter_list);
				symbol_table_instance.add_in_one_entry(node->get_leftmost_child()->get_id(), "PROCEDURE", "NONE", scope_stack.front(), -1, parameter_list, NULL);
				node->get_leftmost_child()->set_is_traversed();
				break;
			}
			case NODE_FUNCTION:
			{
				ParameterList parameter_list;
				Node* lparen_node = node->get_leftmost_child()->get_rsibling()->get_leftmost_child();
				string return_type(get_nodeType_name(node->get_leftmost_child()->get_node_type()));
				TraverseFunctionDeclareParameter(lparen_node->get_leftmost_child(), parameter_list);
				symbol_table_instance.add_in_one_entry(node->get_leftmost_child()->get_leftmost_child()->get_id(), "FUNCTION", return_type, scope_stack.front(), -1, parameter_list, NULL);
				node->get_leftmost_child()->set_is_traversed();
				break;
			}
			case NODE_INT_WORD:
			case NODE_REAL_WORD:
			case NODE_STRING_WORD:
			{
				Node* temp = node->get_leftmost_child();
				do
				{
					int type = node->get_node_type();
					string s_temp;
					if(! symbol_table_instance.add_in_one_entry(temp->get_id(), get_nodeType_name(type), "NONE", scope_stack.front(), 0, ParameterList(), NULL) )
						cout << "[ERROR] Redefined variable " << temp->get_id() << " at line " << temp->get_line_no() << endl;
					temp = temp->get_rsibling();
				} while(temp != NULL);
				return;
				break;
			}
			case NODE_ARRAY:
			{
				Node* temp = node;
				int array_dimension = 1;
				while(temp->get_node_array_type()->get_node_type() == NODE_ARRAY)
				{
					array_dimension++;
					temp = temp->get_node_array_type();
				}
				temp = node->get_leftmost_child();
				do
				{
					int type = node->get_node_type();
					string s_temp;
					if(! symbol_table_instance.add_in_one_entry(temp->get_id(), get_nodeType_name(type), "NONE", scope_stack.front(), array_dimension, ParameterList(), node->get_node_array_type() ) )
						cout << "[ERROR] Redefined variable " << temp->get_id() << " at line " << temp->get_line_no() << endl;
					temp = temp->get_rsibling();
				} while(temp != NULL);
				return;
				break;
			}
			case NODE_ASSIGNMENT:
			{
				//cout << "ASSIGNMENT" << endl;
				Node* left_node = node->get_leftmost_child();
				Node* right_node = node->get_leftmost_child()->get_rsibling();
				if( symbol_table_instance.search_entry(node->get_leftmost_child()->get_id()) )
				{
					traversal(right_node); //Run expression
					if("ARRAY" != symbol_table_instance.get_ExistingEntry_DataType(left_node))
					{//LHS is not a array type identifier
						left_node->set_data_type(symbol_table_instance.get_ExistingEntry_DataType(left_node));
						if(calculate_array_current_dimension(left_node))
							cout << "[ERROR] " << left_node->get_id() << " at line " << left_node->get_line_no() << " is not array type" << endl;
						if(! DataTypeChecking(left_node, right_node))
							cout << "[ERROR] Type error at line " << left_node->get_line_no() << endl;
					}
					else
					{//LHS is a array type identifier
						int current_dimension = 0;
						if(! array_current_dimesion_check(left_node))//Dimension not match
							cout << "[ERROR] Dimension error at line " << left_node->get_line_no() << endl;
						else
						{//Dimension match
							if(get_array_basic_DataType(symbol_table_instance.get_existing_entry(left_node)->array_type) != right_node->get_data_type()) //Basic type
								cout << "[ERROR] Type error at line " << left_node->get_line_no() << endl;
						}
					}
				}
				else
				{//LHS is not found
					cout << "[ERROR] Undeclared Variable \"" << node->get_leftmost_child()->get_id()  << "\" at line " << node->get_leftmost_child()->get_line_no() << endl;
				}
				left_node->set_is_traversed();
				right_node->set_is_traversed();
				/*if(node->get_rsibling() == NULL)
					return;*/
				break;
			}
			case OP_ADD:
			case OP_SUB:
			{
				Node* left_child = node->get_leftmost_child();
				Node* right_child = node->get_leftmost_child()->get_rsibling();
				string lhs_type, rhs_type;
				traversal(left_child);
				traversal(right_child);
				if(left_child->get_node_type() == NODE_IDENTIFIER)
				{
					if(!symbol_table_instance.search_entry(left_child->get_id()))
						cout << "[ERROR] Undeclared Variable \"" << left_child->get_leftmost_child()->get_id()  << "\" at line " << left_child->get_line_no() << endl;
					else
					{
						if(symbol_table_instance.get_ExistingEntry_DataType(left_child) == "ARRAY")
						{
							if(!array_current_dimesion_check(left_child))
								cout << "[ERROR] Dimension error " << left_child->get_line_no() << endl;
							else
								lhs_type = get_array_basic_DataType(symbol_table_instance.get_existing_entry(left_child)->array_type);
						}
						else if(left_child->get_leftmost_child() != NULL)
						{
							if(left_child->get_leftmost_child()->get_node_type() != NODE_LPAREN)
							{
								if(calculate_array_current_dimension(left_child))
									cout << "[ERROR] " << left_child->get_id() << " at line " << left_child->get_line_no() << " is not array type" << endl;
								else
									lhs_type = symbol_table_instance.get_ExistingEntry_DataType(left_child);
							}
						}
						else
						{
							lhs_type = symbol_table_instance.get_ExistingEntry_DataType(left_child);
						}
					}
				}
				else
					lhs_type = left_child->get_data_type();

				if(right_child == NULL)
					rhs_type = lhs_type;
				else if(right_child->get_node_type() == NODE_IDENTIFIER)
				{
					if(!symbol_table_instance.search_entry(right_child->get_id()))
						cout << "[ERROR] Undeclared Variable \"" << right_child->get_leftmost_child()->get_id()  << "\" at line " << right_child->get_line_no() << endl;
					else
					{
						if(symbol_table_instance.get_ExistingEntry_DataType(right_child) == "ARRAY")
						{
							if(!array_current_dimesion_check(right_child))
								cout << "[ERROR] Dimension error " << right_child->get_line_no() << endl;
							else
								rhs_type = get_array_basic_DataType(symbol_table_instance.get_existing_entry(right_child)->array_type);
						}
						else if(right_child->get_leftmost_child() != NULL)
						{
							if(right_child->get_leftmost_child()->get_node_type() != NODE_LPAREN)
							{
								if(calculate_array_current_dimension(right_child))
									cout << "[ERROR] " << right_child->get_id() << " at line " << right_child->get_line_no() << " is not array type" << endl;
								else
									rhs_type = symbol_table_instance.get_ExistingEntry_DataType(right_child);
							}
							else
								rhs_type = symbol_table_instance.get_ExistingEntry_DataType(right_child);
						}
						else
						{
							rhs_type = symbol_table_instance.get_ExistingEntry_DataType(right_child);
						}
					}
				}
				else
					rhs_type = right_child->get_data_type();

				if(lhs_type == "REAL" || rhs_type == "REAL")
					node->set_data_type("REAL");
				else
						node->set_data_type("INTEGER");
				break;
			}
			case OP_MUL:
			{
				Node* left_child = node->get_leftmost_child();
				Node* right_child = node->get_leftmost_child()->get_rsibling();
				string lhs_type, rhs_type;
				traversal(left_child);
				traversal(right_child);
				if(left_child->get_node_type() == NODE_IDENTIFIER)
				{
					if(!symbol_table_instance.search_entry(left_child->get_id()))
						cout << "[ERROR] Undeclared Variable \"" << left_child->get_leftmost_child()->get_id()  << "\" at line " << left_child->get_line_no() << endl;
					else
					{
						if(symbol_table_instance.get_ExistingEntry_DataType(left_child) == "ARRAY")
						{
							if(!array_current_dimesion_check(left_child))
								cout << "[ERROR] Dimension error " << left_child->get_line_no() << endl;
							else
								lhs_type = get_array_basic_DataType(symbol_table_instance.get_existing_entry(left_child)->array_type);
						}
						else if(left_child->get_leftmost_child() != NULL)
						{
							if(left_child->get_leftmost_child()->get_node_type() != NODE_LPAREN)
							{
								if(calculate_array_current_dimension(left_child))
									cout << "[ERROR] " << left_child->get_id() << " at line " << left_child->get_line_no() << " is not array type" << endl;
								else
									lhs_type = symbol_table_instance.get_ExistingEntry_DataType(left_child);
							}
							else
							{
								lhs_type = symbol_table_instance.get_ExistingEntry_DataType(left_child);
							}
						}
						else
						{
							lhs_type = symbol_table_instance.get_ExistingEntry_DataType(left_child);
						}
					}
				}
				else
					lhs_type = left_child->get_data_type();

				if(right_child->get_node_type() == NODE_IDENTIFIER)
				{
					if(!symbol_table_instance.search_entry(right_child->get_id()))
						cout << "[ERROR] Undeclared Variable \"" << right_child->get_leftmost_child()->get_id()  << "\" at line " << right_child->get_line_no() << endl;
					else
					{
						if(symbol_table_instance.get_ExistingEntry_DataType(right_child) == "ARRAY")
						{
							if(!array_current_dimesion_check(right_child))
								cout << "[ERROR] Dimension error " << right_child->get_line_no() << endl;
							else
								rhs_type = get_array_basic_DataType(symbol_table_instance.get_existing_entry(right_child)->array_type);
						}
						else if(right_child->get_leftmost_child() != NULL)
						{
							if(right_child->get_leftmost_child()->get_node_type() != NODE_LPAREN)
							{
								if(calculate_array_current_dimension(right_child))
									cout << "[ERROR] " << right_child->get_id() << " at line " << right_child->get_line_no() << " is not array type" << endl;
								else
									rhs_type = symbol_table_instance.get_ExistingEntry_DataType(right_child);
							}
							else
								rhs_type = symbol_table_instance.get_ExistingEntry_DataType(right_child);
						}
						else
						{
							rhs_type = symbol_table_instance.get_ExistingEntry_DataType(right_child);
						}
					}
				}
				else
					rhs_type = right_child->get_data_type();

				if(lhs_type == "REAL" || rhs_type == "REAL")
					node->set_data_type("REAL");
				else
						node->set_data_type("INTEGER");
				break;
			}
			case OP_DIV:
			{
				Node* left_child = node->get_leftmost_child();
				Node* right_child = node->get_leftmost_child()->get_rsibling();
				string lhs_type, rhs_type;
				traversal(left_child);
				traversal(right_child);
				if(left_child->get_node_type() == NODE_IDENTIFIER)
				{
					if(!symbol_table_instance.search_entry(left_child->get_id()))
						cout << "[ERROR] Undeclared Variable \"" << left_child->get_leftmost_child()->get_id()  << "\" at line " << left_child->get_line_no() << endl;
					else
					{
						if(symbol_table_instance.get_ExistingEntry_DataType(left_child) == "ARRAY")
						{
							if(!array_current_dimesion_check(left_child))
								cout << "[ERROR] Dimension error " << left_child->get_line_no() << endl;
							else
								lhs_type = get_array_basic_DataType(symbol_table_instance.get_existing_entry(left_child)->array_type);
						}
						else if(left_child->get_leftmost_child() != NULL)
						{
							if(left_child->get_leftmost_child()->get_node_type() != NODE_LPAREN)
							{
								if(calculate_array_current_dimension(left_child))
									cout << "[ERROR] " << left_child->get_id() << " at line " << left_child->get_line_no() << " is not array type" << endl;
								else
									lhs_type = symbol_table_instance.get_ExistingEntry_DataType(left_child);
							}
							else
								lhs_type = symbol_table_instance.get_ExistingEntry_DataType(left_child);
						}
						else
						{
							lhs_type = symbol_table_instance.get_ExistingEntry_DataType(left_child);
						}
					}
				}
				else
					lhs_type = left_child->get_data_type();

				if(right_child->get_node_type() == NODE_IDENTIFIER)
				{
					if(!symbol_table_instance.search_entry(right_child->get_id()))
						cout << "[ERROR] Undeclared Variable \"" << right_child->get_leftmost_child()->get_id()  << "\" at line " << right_child->get_line_no() << endl;
					else
					{
						if(symbol_table_instance.get_ExistingEntry_DataType(right_child) == "ARRAY")
						{
							if(!array_current_dimesion_check(right_child))
								cout << "[ERROR] Dimension error " << right_child->get_line_no() << endl;
							else
								rhs_type = get_array_basic_DataType(symbol_table_instance.get_existing_entry(right_child)->array_type);
						}
						else if(right_child->get_leftmost_child() != NULL)
						{
							if(right_child->get_leftmost_child()->get_node_type() != NODE_LPAREN)
							{
								if(calculate_array_current_dimension(right_child))
									cout << "[ERROR] " << right_child->get_id() << " at line " << right_child->get_line_no() << " is not array type" << endl;
								else
									rhs_type = symbol_table_instance.get_ExistingEntry_DataType(right_child);
							}
							else
								rhs_type = symbol_table_instance.get_ExistingEntry_DataType(right_child);
						}
						else
						{
							rhs_type = symbol_table_instance.get_ExistingEntry_DataType(right_child);
						}
					}
				}
				else
					rhs_type = right_child->get_data_type();

				if(lhs_type == "INTEGER" && rhs_type == "INTEGER")
					node->set_data_type("INTEGER");
				else
					node->set_data_type("REAL");
				break;
			}
			case NODE_IDENTIFIER:
			{
				//cout << "ID : " << node->get_id() << endl;
				if(! symbol_table_instance.search_entry(node->get_id()))
					cout << "[ERROR] Undeclared Variable \"" << node->get_id()  << "\" at line " << node->get_line_no() << endl;
				else
				{
					if(node->get_leftmost_child()==NULL)
						node->set_data_type(symbol_table_instance.get_ExistingEntry_DataType(node));
					else
					{
						if(node->get_leftmost_child()->get_node_type() == NODE_LPAREN)
						{
							Node* lparen_node = node->get_leftmost_child();
							ParameterList function_call_parameter_list;
							TraverseFunctionCallParameter(lparen_node->get_leftmost_child(), function_call_parameter_list);
							if(!(function_call_parameter_list==(symbol_table_instance.get_existing_entry(node)->parameter_list)))
								cout << "[ERROR] Function Parameter not matched at line " << node->get_line_no() << endl;
							lparen_node->set_is_traversed();
						}
					}
				}
				break;
			}
		}
	}
	if(node->get_leftmost_child() != NULL && !node->get_is_traversed())
	{
		traversal(node->get_leftmost_child());
	}
	if(node->get_rsibling() != NULL)
	{
		traversal(node->get_rsibling());
	}
	if(node->get_node_type() == NODE_BLOCK)
	{
		scope_stack.pop_front();
	}
	return;
}