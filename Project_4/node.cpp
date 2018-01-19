#include "node.h"
using namespace std;

Node::Node()
{
	this->is_traversed = false;
}
Node::Node(int node_type)
{
	this->node_type = node_type;
	this->parent = NULL;
	this->leftmost_child = NULL;
	this->leftmost_sibling = this;
	this->rsibling = NULL;
	this->block_type = NotBlock;
	this->is_traversed = false;
	this->line_no = 0;
	this->array_type = NULL;
	this->local_serial_number = -1;
}

void Node::modify_block_type(BlockType block_type)
{
	this->block_type = block_type;
}
void Node::set_id(const string id)
{
	this->id = id;
}
void Node::set_integer_value(const int integer_value)
{
	this->integer_value = integer_value;
}
void Node::set_real_value(const double real_value)
{
	this->real_value = real_value;
}
void Node::set_array_start_index(const int array_start_index)
{
	this->array_start_index = array_start_index;
}
void Node::set_array_end_index(const int array_end_index)
{
	this->array_end_index = array_end_index;
}
void Node::set_line_no(const int line_no)
{
	this->line_no = line_no;
}
void Node::set_is_traversed(void)
{
	this->is_traversed = true;
}
void Node::set_data_type(string data_type)
{
	this->data_type = data_type;
}
void Node::set_array_type(Node* array_type)
{
	this->array_type = array_type;
}
void Node::set_local_serial_number(int local_serial_number)
{
	this->local_serial_number = local_serial_number;
}
void Node::modify_leftmost_child(Node* leftmost_child)
{
	this->leftmost_child = leftmost_child;
}
void Node::modify_parent(Node* parent)
{
	this->parent = parent;
}
void Node::modify_leftmost_sibling(Node* leftmost_sibling)
{
	this->leftmost_sibling = leftmost_sibling;
}
void Node::modify_rsibling(Node* rsibling)
{
	this->rsibling = rsibling;
}
void Node::clear_traversed(void)
{
	this->is_traversed = false;
}
string Node::get_id(void) const
{
	return this->id;
}
int Node::get_integer_value(void) const
{
	return this->integer_value;
}
double Node::get_real_value(void) const
{
	return this->real_value;
}
int Node::get_array_start_index(void) const
{
	return this->array_start_index;
}
int Node::get_array_end_index(void) const
{
	return this->array_end_index;
}
bool Node::get_is_traversed(void) const
{
	return this->is_traversed;
}
int Node::get_block_type(void) const
{
	return this->block_type;
}
int Node::get_node_type(void) const
{
	return this->node_type;
}
int Node::get_line_no(void) const
{
	return this->line_no;
}
int Node::get_local_serial_number(void) const
{
	return this->local_serial_number;
}
string Node::get_data_type(void) const
{
	return this->data_type;
}
Node* Node::get_node_array_type(void) const
{
	return this->array_type;
}
Node* Node::get_parent(void) const
{
	return this->parent;
}
Node* Node::get_leftmost_child(void) const
{
	return this->leftmost_child;
}
Node* Node::get_leftmost_sibling(void) const
{
	return this->leftmost_sibling;
}
Node* Node::get_rsibling(void) const
{
	return this->rsibling;
}
void MakeSiblings(Node* master, Node* node_add_in)
{	/*Class version*/
	Node* master_temp = master;
	Node* node_add_in_leftmost_sibling = node_add_in->get_leftmost_sibling();
	Node* temp = NULL;
	while(master_temp->get_rsibling() != NULL)
	{
		master_temp = master_temp-> get_rsibling();
	}
	master_temp->modify_rsibling(node_add_in_leftmost_sibling);
	node_add_in_leftmost_sibling->modify_leftmost_sibling(master_temp->get_leftmost_sibling());
	node_add_in_leftmost_sibling->modify_parent(master_temp->get_parent());
	temp = node_add_in_leftmost_sibling;
	while(temp->get_rsibling() != NULL)
	{
		temp = temp->get_rsibling();
		temp->modify_leftmost_sibling(master_temp->get_leftmost_sibling());
		temp->modify_parent(master_temp->get_parent());
	}
}

void AdoptChild(Node* master, Node* child_node_add_in)
{
	if(master->get_leftmost_child() == NULL)
	{
		Node* child_node_add_in_leftmost_sibling = child_node_add_in->get_leftmost_sibling();
		master->modify_leftmost_child(child_node_add_in_leftmost_sibling);
		while(child_node_add_in_leftmost_sibling != NULL)
		{
			child_node_add_in_leftmost_sibling->modify_parent(master);
			child_node_add_in_leftmost_sibling = child_node_add_in_leftmost_sibling->get_rsibling();
		}
	}
	else
	{
		MakeSiblings(master->get_leftmost_child(), child_node_add_in);
	}
}

Node* newNode(int type)
{
	return new Node(type);
}

/*void deleteNode(NODE *node)
{
	if(node==NULL)
	{
		return;
	}
	while(node->leftmost_child)
	{
		deleteNode(node->leftmost_child);
	}
	if(node->string != NULL)
		free(node->string);
	free(node);
}*/


void printTree(Node* node, int ident)
{
	/*Class Version*/
	static char blank[2048];
	for(int i=0; i<ident; i++)
		blank[i] = '\t';
	blank[ident] = 0;
	switch(node->get_node_type())
	{
		case OP_ADD:
			cout << blank << "+" << endl;
			//ident+=1;
			break;
		case OP_SUB:
			cout << blank << "-" << endl;
			//ident+=1;
			break;
		case OP_MUL:
			cout << blank << "*" << endl;
			//ident+=1;
			break;
		case OP_DIV:
			cout << blank << "/" << endl;
			//ident+=1;
			break;
		case OP_LT:
			cout << blank << "<" << endl;
			//ident+=1;
			break;
		case OP_GT:
			cout << blank << ">" << endl;
			//ident+=1;
			break;
		case OP_EQUAL:
			cout << blank << "/=" << endl;
			//ident+=1;
			break;
		case OP_LE:
			cout << blank << "/<=" << endl;
			//ident+=1;
			break;
		case OP_GE:
			cout << blank << "/>=" << endl;
			//ident+=1;
			break;
		case OP_NOTEQUAL:
			cout << blank << "/!=" << endl;
			//ident+=1;
			break;
		case NODE_ARRAY:
			cout << blank << "/ARRAY, " << node->get_array_start_index() << " " << node->get_array_end_index() << endl;
			//ident+=1;
			break;
		case NODE_DO:
			cout << blank << "/DO" << endl;
			//ident+=1;
			break;
		case NODE_ELSE:
			cout << blank << "/ELSE" << endl;
			//ident+=1;
			break;
		case NODE_END:
			cout << blank << "/END" << endl;
			//ident+=1;
			break;
		case NODE_FUNCTION:
			cout << blank << "/FUNCTION" << endl;
			//ident+=1;
			break;
		case NODE_IF:
			cout << blank << "/IF" << endl;
			//ident+=1;
			break;
		case NODE_NOT:
			cout << blank << "/NOT" << endl;
			//ident+=1;
			break;
		case NODE_OF:
			cout << blank << "/OF" << endl;
			//ident+=1;
			break;
		case NODE_PBEGIN:
			cout << blank << "/BEGIN" << endl;
			//ident+=1;
			break;
		case NODE_PROCEDURE:
			cout << blank << "/PROCEDURE" << endl;
			//ident+=1;
			break;
		case NODE_PROGRAM:
			cout << blank << "/PROGRAM" << endl;
			//ident+=1;
			break;
		case NODE_THEN:
			cout << blank << "/THEN" << endl;
			//ident+=1;
			break;
		case NODE_VAR:
			cout << blank << "/VAR" << endl;
			//ident+=1;
			break;
		case NODE_WHILE:
			cout << blank << "/WHILE" << endl;
			//ident+=1;
			break;
		case NODE_IDENTIFIER:
			cout << blank << "/IDENTIFIER " << node->get_id() << endl;
			//ident+=1;
			break;
		case NODE_ASSIGNMENT:
			cout << blank << "/ASSIGNMENT" << endl;
			//ident+=1;
			break;
		case NODE_COLON:
			cout << blank << "/COLON" << endl;
			//ident+=1;
			break;
		case NODE_COMMA:
			cout << blank << "/COMMA" << endl;
			//ident+=1;
			break;
		case NODE_INTEGER:
			cout << blank << "/INTEGER, " << node->get_integer_value() << endl;
			//ident+=1;
			break;
		case NODE_DOT:
			cout << blank << "/DOT" << endl;
			//ident+=1;
			break;
		case NODE_DOTDOT:
			cout << blank << "/DOTDOT" << endl;
			//ident+=1;
			break;
		case NODE_LBRAC:
			cout << blank << "/[" << endl;
			//ident+=1;
			break;
		case NODE_LPAREN:
			cout << blank << "/(" << endl;
			//ident+=1;
			break;
		case NODE_RBRAC:
			cout << blank << "/]" << endl;
			//ident+=1;
			break;
		case NODE_REALNUMBER:
			cout << blank << "/REALNUMBER, " << node->get_real_value() << endl;
			//ident+=1;
			break;
		case NODE_RPAREN:
			cout << blank << "/)" << endl;
			//ident+=1;
			break;
		case NODE_SEMICOLON:
			cout << blank << "/;" << endl;
			//ident+=1;
			break;
		case NODE_NUMBER:
			cout << blank << "/NUMBER, " << node->get_id() << endl;
			//ident+=1;
			break;
		case NODE_INT_WORD:
			cout << blank << "/INT_WORD" << endl;
			//ident+=1;
			break;
		case NODE_REAL_WORD:
			cout << blank << "/REAL_WORD" << endl;
			//ident+=1;
			break;
		case NODE_STRING_WORD:
			cout << blank << "/STRING_WORD" << endl;
			//ident+=1;
			break;
		case NODE_SCIENTIFIC_EXP:
			cout << blank << "/SCIENTIFIC_EXP, " << node->get_real_value() << endl;
			//ident+=1;
			break;
		case NODE_SCIENTIFIC_EXP_EXPON:
			cout << blank << "/SCIENTIFIC_EXP_EXPON" << endl;
			//ident+=1;
			break;
		case NODE_PROG:
			cout << blank << "/PROG" << endl;
			//ident+=1;
			break;
		case NODE_DECLARATIONS:
			cout << blank << "/DECLARATIONS" << endl;
			//ident+=1;
			break;
		case NODE_TYPE:
			cout << blank << "/TYPE" << endl;
			//ident+=1;
			break;
		case NODE_STANDARD_TYPE:
			cout << blank << "/STANDARD_TYPE" << endl;
			//ident+=1;
			break;
		case NODE_SUBDECLARATIONS:
			cout << blank << "/SUBDECLARATIONS" << endl;
			//ident+=1;
			break;
		case NODE_SUBDECLARATION:
			cout << blank << "/SUBDECLARATION" << endl;
			//ident+=1;
			break;
		case NODE_SUBPROGRAM_HEAD:
			cout << blank << "/SUBPROGRAM_HEAD" << endl;
			//ident+=1;
			break;
		case NODE_ARGUMENTS:
			cout << blank << "/ARGUMENTS" << endl;
			//ident+=1;
			break;
		case NODE_PARAM_LIST:
			cout << blank << "/PARAMETER_LIST" << endl;
			//ident+=1;
			break;
		case NODE_OPTIONAL_VAR:
			cout << blank << "/OPTIONAL_VAR" << endl;
			//ident+=1;
			break;
		case NODE_COMP_STATE:
			cout << blank << "/COMPOUND_STATEMENT" << endl;
			//ident+=1;
			break;
		case NODE_OPTIONAL_STATE:
			cout << blank << "/OPTIONAL_STATEMENTS" << endl;
			//ident+=1;
			break;
		case NODE_STATEMENT_LIST:
			cout << blank << "/STATEMENT_LIST" << endl;
			//ident+=1;
			break;
		case NODE_STATEMENT:
			cout << blank << "/STATEMENT" << endl;
			//ident+=1;
			break;
		case NODE_VARIABLE:
			cout << blank << "/VARIABLE" << endl;
			//ident+=1;
			break;
		case NODE_TAIL:
			cout << blank << "/TAIL" << endl;
			//ident+=1;
			break;
		case NODE_PROC_STATE:
			cout << blank << "/PROCEDURE_STATEMENTS" << endl;
			//ident+=1;
			break;
		case NODE_EXPR_LIST:
			cout << blank << "/EXPRESSION_LIST" << endl;
			//ident+=1;
			break;
		case NODE_EXPR:
			cout << blank << "/EXPRESSION" << endl;
			//ident+=1;
			break;
		case NODE_SIM_EXPR:
			cout << blank << "/SIMPLE_EXPRESSION" << endl;
			//ident+=1;
			break;
		case NODE_TERM:
			cout << blank << "/TERM" << endl;
			//ident+=1;
			break;
		case NODE_FACTOR:
			cout << blank << "/FACTOR" << endl;
			//ident+=1;
			break;
		case NODE_ADDOP:
			cout << blank << "/ADDOP" << endl;
			//ident+=1;
			break;
		case NODE_MULOP:
			cout << blank << "/MULOP" << endl;
			//ident+=1;
			break;
		case NODE_RELAOP:
			cout << blank << "/REALOP" << endl;
			//ident+=1;
			break;
		case NODE_IDENTIFIER_LIST:
			cout << blank << "/IDENTIFIER_LIST" << endl;
			//ident+=1;
			break;
		case NODE_LAMBDA:
			cout << blank << "/LAMBDA" << endl;
			//ident+=1;
			break;
		case NODE_BLOCK:
			cout << blank << "/BLOCK, " << node->get_block_type() << endl;
			//ident+=1;
			break;
		case NODE_ARRAY_UPPER_BOUND:
			cout << blank << "/ARRAY_UPPER_BOUND" << endl;
			//ident+=1;
			break;
		case NODE_ARRAY_LOWER_BOUND:
			cout << blank << "/ARRAY_LOWER_BOUND" << endl;
			//ident+=1;
			break;
		default:
			cout << "This is a default value" << endl;
			cout << blank << "Default" << endl;
			break;
	}

	if(node->get_leftmost_child() != NULL)
	{
		printTree(node->get_leftmost_child(), ident+1);
	}
	if(node->get_rsibling() != NULL)
	{
		printTree(node->get_rsibling(), ident);
	}
}
