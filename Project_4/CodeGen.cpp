#include "CodeGen.h"

using namespace std;
extern int scope_codegen_used;
extern SymbolTable symbol_table_instance;
extern map<string, string> FunctionNameList;
extern map<string, string> ProcedureNameList;
extern deque< deque<string> > if_while_instruction_stack;
typedef map<string, string>::iterator FUNC_PROC_ITERATOR;
CODEGEN::CODEGEN(){};
CODEGEN::CODEGEN(string program_name)
{
	int last_slash_pos = program_name.find_last_of("/");
	int last_dot_pos = program_name.find_last_of(".");
	this->program_name = program_name.substr(0, last_dot_pos);
	this->Open_J_File();
	this->program_name = program_name.substr(last_slash_pos + 1, last_dot_pos - last_slash_pos - 1);
	this->PrintCommonPart();
}
string CODEGEN::getProgramName(void) const
{
	return this->program_name;
}
int CODEGEN::get_total_scope(void) const
{
	return this->total_scope;
}
int CODEGEN::get_current_scope(void) const
{
	return this->scope_stack_codegen_used.front();
}
int CODEGEN::get_scope_stack_size(void) const
{
	return this->scope_stack_codegen_used.size();
}
bool CODEGEN::search_from_symbol_table(string id, SymbolTable* symbol_table_ptr) const
{
	return symbol_table_ptr->search_entry(id, this->scope_stack_codegen_used);
}
vector<ENTRY>::const_iterator CODEGEN::get_existing_entry_from_symtab(Node* node, SymbolTable* symbol_table_ptr) const
{
	return symbol_table_ptr->get_existing_entry(node, this->scope_stack_codegen_used);
}
void CODEGEN::PrintStackSize(int size)
{
	this->PrintInstruction(".limit stack " + numeric2string<int>(size));
}
void CODEGEN::PrintLocalSize(int size)
{
	this->PrintInstruction(".limit locals " + numeric2string<int>(size));
}
void CODEGEN::Close_J_File(void)
{
	this->fin.close();
}
void CODEGEN::SetProgramName(string program_name)
{
	this->program_name = program_name;
}
void CODEGEN::SetProgramName(char* program_name)
{
	string temp(program_name);
	this->program_name = temp;
}
void CODEGEN::PrintInstruction(string instrunction)
{
	this->fin << instrunction << endl;
}
void CODEGEN::push_front_scope_stack_codegen_used(int scope)
{
	this->scope_stack_codegen_used.push_front(scope);
}
void CODEGEN::pop_front_scope_stack_codegen_used(void)
{
	this->scope_stack_codegen_used.pop_front();
}
void CODEGEN::total_scope_increment_one(void)
{
	this->total_scope++;
}
void CODEGEN::Open_J_File(void)
{
	cout << "this->program_name : " << this->program_name << endl;
	this->fin.open( (this->program_name + ".j").c_str(), ios::out | ios::trunc);
}
void CODEGEN::PrintCommonPart(void)
{
	fin << "; Program name : " << this->program_name << endl;
	fin << ".class public " << this->program_name << endl;
	fin << ".super java/lang/Object" << endl;
	fin << ".field public static _sc Ljava/util/Scanner;" << endl;
}

string toLowerCase(string target)
{
	for(unsigned int i = 0; i < target.length(); i++)
	{
		if(target[i] >= 'A' && target[i] < 'Z')
			target[i] = target[i] + 32;
	}
	return target;
}

template<class T> string numeric2string(T value)
{
	stringstream ss;
	ss << value;
	string result;
	ss >> result;
	return result;
}

void ClearTraversed(Node* node_start)
{
	node_start->clear_traversed();
	if(node_start->get_leftmost_child() != NULL)
		ClearTraversed(node_start->get_leftmost_child());
	if(node_start->get_rsibling() != NULL)
		ClearTraversed(node_start->get_rsibling());
	return;
}

int get_array_dimension(Node* array_node)
{
	int dimension = 0;
	while(array_node->get_node_type() == NODE_ARRAY)
	{
		dimension++;
		array_node = array_node->get_leftmost_child();
	}
	return dimension;
}

void LHS_handler(Node* lhs_node, CODEGEN* codegen_ptr)
{
	/****************************************
	LHS means left hand side of assignment which can only be identifier
	*****************************************/
	/*if(lhs_node->get_node_type() == NODE_IDENTIFIER)
	{
		vector<ENTRY>::const_iterator result = codegen_ptr->get_existing_entry_from_symtab(lhs_node, &symbol_table_instance);
		string type = (result->type) == "INTEGER" ? "I" : "F";
		if( result->belong_scope == 0 )
		{//Global variable
			codegen_ptr->PrintInstruction("putstatic " + codegen_ptr->getProgramName() + "/" + lhs_node->get_id() + " " + type);
		}
		else
		{//Array accessing
			cout << "20 " << endl;
		}
	}*/
	vector<ENTRY>::const_iterator result = codegen_ptr->get_existing_entry_from_symtab(lhs_node, &symbol_table_instance);
	if( result->belong_scope == 0)
	{//Global variable
		if( result->type == "INTEGER" )
			codegen_ptr->PrintInstruction("putstatic " + codegen_ptr->getProgramName() + "/" + lhs_node->get_id() + " I");
		else if ( result->type == "REAL" )
			codegen_ptr->PrintInstruction("putstatic " + codegen_ptr->getProgramName() + "/" + lhs_node->get_id() + " F");
		else if ( result->type == "FUNCTION" )
		{
			if( result->return_type == "INTEGER" )
				codegen_ptr->PrintInstruction("putstatic " + codegen_ptr->getProgramName() + "/" + lhs_node->get_id() + " I");
			else
				codegen_ptr->PrintInstruction("putstatic " + codegen_ptr->getProgramName() + "/" + lhs_node->get_id() + " F");
		}
		else if ( result->type == "STRING" )
			codegen_ptr->PrintInstruction("putstatic " + codegen_ptr->getProgramName() + "/" + lhs_node->get_id() + " Ljava/lang/String;");
		else
			codegen_ptr->PrintInstruction(";which means it's array and I give up");
	}
	else
	{
		if( result->type == "INTEGER" )
			codegen_ptr->PrintInstruction( "istore " + numeric2string<int>( result->local_serial_number ) );
		else if( result->type == "REAL" )
			codegen_ptr->PrintInstruction( "fstore " + numeric2string<int>( result->local_serial_number ) );
		else if ( result->type == "FUNCTION" )
		{
			if( result->return_type == "INTEGER" )
				codegen_ptr->PrintInstruction("putstatic " + codegen_ptr->getProgramName() + "/" + lhs_node->get_id() + " I");
			else
				codegen_ptr->PrintInstruction("putstatic " + codegen_ptr->getProgramName() + "/" + lhs_node->get_id() + " F");
		}
		else if( result->type == "STRING" )
			codegen_ptr->PrintInstruction( "astore " + numeric2string<int>( result->local_serial_number ) );
		else
			codegen_ptr->PrintInstruction(";which means it's array and I give up");
	}
}

string FunctionProcedureParameter_Handler(Node* lparen_node)
{
	string parameter_list("");
	string type;
	if(lparen_node->get_node_type() != NODE_LPAREN)
	{
		return type;
	}
	Node* lambda_node = lparen_node->get_leftmost_child();
	Node* type_node = NULL;
	Node* temp = NULL;
	while(lambda_node != NULL)
	{
		type_node = lambda_node->get_leftmost_child();
		type = type_node->get_node_type() == NODE_INT_WORD ? "I" : "F";
		//Parameter type decision by using determine NODE_INT_WORD or NODE_REAL_WORD
		temp = type_node->get_leftmost_child();
		while(temp != NULL)
		{
			parameter_list += type;
			temp = temp->get_rsibling();
		}
		lambda_node = lambda_node->get_rsibling();
	}
	return parameter_list;
}
/*
----------------------------------------
|	Predicate_Handler return value		|
-----------------------------------------
|	0			|	relation compare	|
|	1			|	arithmetic operation|
|	2			|	constant			|
-----------------------------------------
*/
int Predicate_Handler(Node* predicate_start_node)
{
	int result;
	switch (predicate_start_node->get_node_type())
	{
		case OP_LT:
		case OP_GT:
		case OP_LE:
		case OP_GE:
		case OP_EQUAL:
		case OP_NOTEQUAL:
		{
			result = 0;
			break;
		}
		case OP_ADD:
		case OP_SUB:
		case OP_MUL:
		case OP_DIV:
		{
			result = 1;
			break;
		}
		case NODE_INTEGER:
		case NODE_REALNUMBER:
		{
			result = 2;
			break;
		}
		default:
		{
			result = 3;
			break;
		}
	}
	return result;
}

void Operand_TypeCasting(Node* op1, CODEGEN* codegen_ptr)
{
	Node* op2 = op1->get_rsibling();
	CodeGen_Traversal(op1, codegen_ptr);
	//CodeGen_Traversal(op2, codegen_ptr);
	if(op1->get_data_type() == "REAL" && op2->get_data_type() == "INTEGER")
		codegen_ptr->PrintInstruction("i2f");
	else if(op2->get_data_type() == "REAL" && op1->get_data_type() == "INTEGER")
	{
		codegen_ptr->PrintInstruction("fstore 80");
		codegen_ptr->PrintInstruction("i2f");
		codegen_ptr->PrintInstruction("fload 80");
	}
	else
		op1 = op1;
}

void CodeGen_Traversal(Node* node_start, CODEGEN* codegen_ptr)
{
	cout << "node_start type : " << node_start->get_node_type() << endl;
	if(node_start == NULL)
		return;
	if(node_start->get_is_traversed())
		cout << "Traversed" << endl;
	if(node_start->get_is_traversed() == false)
	{
		switch(node_start->get_node_type())
		{
			case NODE_BLOCK:
			{
				codegen_ptr->push_front_scope_stack_codegen_used(codegen_ptr->get_total_scope());
				codegen_ptr->total_scope_increment_one();
				scope_codegen_used = codegen_ptr->get_current_scope();
				if(node_start->get_block_type() == PROGRAM_BODY_BLOCK)
				{
					codegen_ptr->PrintInstruction(".method public static main([Ljava/lang/String;)V");
					codegen_ptr->PrintInstruction(".limit stack 400");
					codegen_ptr->PrintInstruction(".limit locals 400");
					codegen_ptr->PrintInstruction("new java/util/Scanner");
					codegen_ptr->PrintInstruction("dup");
					codegen_ptr->PrintInstruction("getstatic java/lang/System/in Ljava/io/InputStream;");
					codegen_ptr->PrintInstruction("invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V");
					codegen_ptr->PrintInstruction("putstatic " + codegen_ptr->getProgramName() + "/_sc Ljava/util/Scanner;");
				}
				break;
			}
			case NODE_PROGRAM:
			{
				node_start->set_is_traversed();
				break;
			}
			case NODE_VAR:
			{
				cout << "scope_codegen_used : " << scope_codegen_used << endl;
				if(scope_codegen_used == 0)
				{//Global variable
					Node* type_node = node_start->get_leftmost_child();
					int type_number = type_node->get_node_type();
					/*string type = ( type_node->get_node_type() == NODE_INT_WORD ? "I" : "F" );
					for(Node* ptr = type_node->get_leftmost_child(); ptr != NULL; ptr = ptr->get_rsibling())
						codegen_ptr->PrintInstruction(".field public static " + ptr->get_id() + " " + type);*/
					if(type_number != NODE_ARRAY)
					{
						string type = "";//( type_node->get_node_type() == NODE_INT_WORD ? "I" : "F" );
						if( type_node->get_node_type() == NODE_INT_WORD )
							type = "I";
						else if( type_node->get_node_type() == NODE_REAL_WORD )
							type = "F";
						else
							type = "Ljava/lang/String;";
						for(Node* ptr = type_node->get_leftmost_child(); ptr != NULL; ptr = ptr->get_rsibling())
							codegen_ptr->PrintInstruction(".field public static " + ptr->get_id() + " " + type);
					}
					else
					{
						string array_brac="";
						int dimension = get_array_dimension(node_start->get_leftmost_child());
						for(int i = 0; i < dimension; i++)
							array_brac += "[";
						if(get_array_basic_DataType(node_start->get_leftmost_child()) == "INTEGER")
							array_brac += "I";
						else
							array_brac += "F";
					}
				}
				else
				{
					Node* type_node = node_start->get_leftmost_child();
					if(type_node->get_node_type() == NODE_STRING_WORD)
					{
						int local_serial_number;
						for(Node* ptr = type_node->get_leftmost_child(); ptr != NULL; ptr = ptr->get_rsibling())
						{
							local_serial_number = ptr->get_local_serial_number();
							codegen_ptr->PrintInstruction("new java/lang/String");//Not reference type, no ";"
							codegen_ptr->PrintInstruction("astore " + numeric2string<int>(local_serial_number));
						}
					}
				}
				node_start->set_is_traversed();
				break;
			}
			case NODE_FUNCTION:
			{
				Node* lparen_node = ((node_start->get_leftmost_child())->get_rsibling())->get_leftmost_child();
				Node* FunctionName_node = node_start->get_leftmost_child()->get_leftmost_child();
				/**/
				vector<ENTRY>::const_iterator result = codegen_ptr->get_existing_entry_from_symtab(FunctionName_node, &symbol_table_instance);
				string return_type = (result->return_type == "INTEGER" ? "I" : "F");
				/*FunctionName is also a global variable which can be used in function body(like static variable)*/
				codegen_ptr->PrintInstruction(".field public static " + FunctionName_node->get_id() + " " + return_type);
				codegen_ptr->PrintInstruction(".method public static " + node_start->get_leftmost_child()->get_leftmost_child()->get_id() + "(" + FunctionProcedureParameter_Handler(lparen_node) + ")" + return_type);
				FunctionNameList.insert( pair<string, string>(FunctionName_node->get_id(), "(" + FunctionProcedureParameter_Handler(lparen_node) + ")" + return_type) );
				codegen_ptr->PrintStackSize(40);
				codegen_ptr->PrintLocalSize(40);
				/*Manually add a new scope, and manually delete this scope later*/
				codegen_ptr->push_front_scope_stack_codegen_used(codegen_ptr->get_total_scope());
				codegen_ptr->total_scope_increment_one();
				scope_codegen_used = codegen_ptr->get_current_scope();
				/*Deal with function body, function name here is kind of identifier and return value*/
					cout << "type : " << lparen_node->get_rsibling()->get_rsibling()->get_node_type() << endl;
					CodeGen_Traversal(lparen_node->get_rsibling()->get_rsibling(), codegen_ptr);
				codegen_ptr->pop_front_scope_stack_codegen_used();
				scope_codegen_used = codegen_ptr->get_current_scope();
				/*Return the value*/
				if(return_type != "")
				{
					codegen_ptr->PrintInstruction("getstatic " + codegen_ptr->getProgramName() + "/" + FunctionName_node->get_id() + " " + return_type);
					codegen_ptr->PrintInstruction(toLowerCase(return_type) + "return");
				}
				else
					codegen_ptr->PrintInstruction("return");
				codegen_ptr->PrintInstruction(".end method");
				node_start->set_is_traversed();
				break;
			}
			case NODE_PROCEDURE:
			{
				Node* lparen_node = ((node_start->get_leftmost_child())->get_rsibling())->get_leftmost_child();
				Node* ProcedureName_node = node_start->get_leftmost_child();
				/*Find the procedure in symbol table*/
				vector<ENTRY>::const_iterator result = codegen_ptr->get_existing_entry_from_symtab(ProcedureName_node, &symbol_table_instance);
				/*ProcedureName is also a global variable which can be used in function body(like static variable)*/
				/*method byte-code gen*/
				codegen_ptr->PrintInstruction(".method public static " + ProcedureName_node->get_id() + "(" + FunctionProcedureParameter_Handler(lparen_node) +")V");
				ProcedureNameList.insert( pair<string, string>(ProcedureName_node->get_id(), "(" + FunctionProcedureParameter_Handler(lparen_node) +")V") );
				codegen_ptr->PrintStackSize(40);
				codegen_ptr->PrintLocalSize(40);
				/*Manually add a new scope, and manually delete this scope later*/
				codegen_ptr->push_front_scope_stack_codegen_used(codegen_ptr->get_total_scope());
				codegen_ptr->total_scope_increment_one();
				scope_codegen_used = codegen_ptr->get_current_scope();
					/*Deal with procedure body*/
					CodeGen_Traversal(lparen_node->get_rsibling()->get_rsibling(), codegen_ptr);
				codegen_ptr->pop_front_scope_stack_codegen_used();
				scope_codegen_used = codegen_ptr->get_current_scope();
				/*void return*/
				codegen_ptr->PrintInstruction("return");
				codegen_ptr->PrintInstruction(".end method");
				node_start->set_is_traversed();
				break;
			}
			case NODE_IF:
			{
				Node* predicate_start_node = node_start->get_leftmost_child();
				Node* if_block_node = predicate_start_node->get_rsibling();
				Node* else_block_node = if_block_node->get_rsibling();
				if_block_node->set_is_traversed();
				else_block_node->set_is_traversed();
				/*Deal with predicate*/
				CodeGen_Traversal(predicate_start_node, codegen_ptr);
				switch (Predicate_Handler(predicate_start_node))
				{
					case 0:
					{//Relation Operation
						codegen_ptr->PrintInstruction(if_while_instruction_stack.front().front());
						if_while_instruction_stack.front().pop_front();
						break;
					}
					case 1:
					{//Arithmetic Operation
						string label_number(numeric2string<int>(if_while_instruction_stack.size()));
						codegen_ptr->PrintInstruction("ifeq Lfalse" + label_number);
						if_while_instruction_stack.push_front( deque<string>(1, "Lfalse" + label_number + ":" ) );
						if_while_instruction_stack.front().push_back("Lexit" + label_number);
						if_while_instruction_stack.front().push_back("Lexit" + label_number + ":");
						break;
					}
					case 2:
					{//Constant
						if(predicate_start_node->get_node_type() == NODE_REALNUMBER)
						{
							codegen_ptr->PrintInstruction("fconst_0");
							codegen_ptr->PrintInstruction("fcmpl");
						}
						string label_number(numeric2string<int>(if_while_instruction_stack.size()));
						codegen_ptr->PrintInstruction("ifeq Lfalse" + label_number);
						if_while_instruction_stack.push_front( deque<string>(1, "Lfalse" + label_number + ":" ) );
						if_while_instruction_stack.front().push_back("Lexit" + label_number);
						if_while_instruction_stack.front().push_back("Lexit" + label_number + ":");
						break;
					}
					default:
					{//Maybe just a identifier or Array Accessing(may not be implementd)
						codegen_ptr->PrintInstruction("i2f");
						codegen_ptr->PrintInstruction("fconst_0");
						codegen_ptr->PrintInstruction("fcmpl");
						string label_number(numeric2string<int>(if_while_instruction_stack.size()));
						codegen_ptr->PrintInstruction("ifeq Lfalse" + label_number);
						if_while_instruction_stack.push_front( deque<string>(1, "Lfalse" + label_number + ":" ) );
						if_while_instruction_stack.front().push_back("Lexit" + label_number);
						if_while_instruction_stack.front().push_back("Lexit" + label_number + ":");
						break;
					}
				}
				/*True body*/
				if_block_node->clear_traversed();
				CodeGen_Traversal(predicate_start_node->get_rsibling(), codegen_ptr);
				/*Print the instrunction for goto Lexit*/
				codegen_ptr->PrintInstruction("goto " + if_while_instruction_stack.front().at(1));
				/*Print false label*/
				codegen_ptr->PrintInstruction( if_while_instruction_stack.front().at(0) );
				/*Else body*/
				else_block_node->clear_traversed();
				CodeGen_Traversal(predicate_start_node->get_rsibling()->get_rsibling(), codegen_ptr);
				/*Print Lexit(x) label*/
				codegen_ptr->PrintInstruction( if_while_instruction_stack.front().at(2) );
				if_while_instruction_stack.pop_front();
				node_start->set_is_traversed();
				break;
			}
			case NODE_WHILE:
			{
				Node* predicate_start_node = node_start->get_leftmost_child();
				Node* while_body_node = predicate_start_node->get_rsibling();
				while_body_node->set_is_traversed();
				/*Deal with predicate*/
				string label_number( numeric2string<int>( if_while_instruction_stack.size() ) );
				codegen_ptr->PrintInstruction("Wbegin" + label_number + ":");
				CodeGen_Traversal(predicate_start_node, codegen_ptr);
				switch (Predicate_Handler(predicate_start_node))
				{
					case 0:
					{//Relation Operation
						int pos = if_while_instruction_stack.front().front().find_last_of(' ');
						codegen_ptr->PrintInstruction( if_while_instruction_stack.front().front().substr(0, pos + 1 ) + "Wexit" + label_number );
						if_while_instruction_stack.front().pop_front();
						if_while_instruction_stack.front().clear();
						if_while_instruction_stack.front().push_back("Wbegin" + label_number);
						if_while_instruction_stack.front().push_back("Wexit" + label_number + ":");
						break;
					}
					case 1:
					{//Arithmetic Operation
						codegen_ptr->PrintInstruction("ifeq Wexit" + label_number);
						if_while_instruction_stack.push_front( deque<string>(1, "Wbegin" + label_number) );
						if_while_instruction_stack.front().push_back("Wexit" + label_number + ":");
						break;
					}
					case 2:
					{//Constant
						if(predicate_start_node->get_node_type() == NODE_REALNUMBER)
						{
							codegen_ptr->PrintInstruction("fconst_0");
							codegen_ptr->PrintInstruction("fcmpl");
						}
						codegen_ptr->PrintInstruction("ifeq Wexit" + label_number);
						if_while_instruction_stack.push_front( deque<string>(1, "Wbegin" + label_number) );
						if_while_instruction_stack.front().push_back("Wexit" + label_number + ":");
						break;
					}
					default:
					{//Maybe just a identifier or Array Accessing(may not be implementd)
						if(predicate_start_node->get_data_type() != "INTEGER")
						{
							codegen_ptr->PrintInstruction("i2f");
							codegen_ptr->PrintInstruction("fconst_0");
							codegen_ptr->PrintInstruction("fcmpl");
						}
						codegen_ptr->PrintInstruction("ifeq Wexit" + label_number);
						if_while_instruction_stack.push_front( deque<string>(1, "Wbegin" + label_number) );
						if_while_instruction_stack.front().push_back("Wexit" + label_number + ":");
						break;
					}
				}
				/*While body*/
				while_body_node->clear_traversed();
				CodeGen_Traversal(predicate_start_node->get_rsibling(), codegen_ptr);
				/*Print the instrunction for goto Wbegin*/
				codegen_ptr->PrintInstruction("goto " + if_while_instruction_stack.front().at(0));
				/*Print the label of Wexit*/
				codegen_ptr->PrintInstruction(if_while_instruction_stack.front().at(1));
				if_while_instruction_stack.pop_front();
				node_start->set_is_traversed();
				break;
			}
			case NODE_ASSIGNMENT:
			{
				//cout << "Inside NODE_ASSIGNMENT" << endl;
				Node* lhs = node_start->get_leftmost_child();
				Node* rhs = node_start->get_leftmost_child()->get_rsibling();
				CodeGen_Traversal(rhs, codegen_ptr);
				LHS_handler(lhs, codegen_ptr);
				node_start->set_is_traversed();
				break;
			}
			case NODE_PRINT:
			case NODE_WRITE:
			{
				Node* child_node = node_start->get_leftmost_child();
				codegen_ptr->PrintInstruction("getstatic java/lang/System/out Ljava/io/PrintStream;");
				CodeGen_Traversal(node_start->get_leftmost_child(), codegen_ptr);
				if( child_node->get_node_type() == NODE_IDENTIFIER)
				{
					vector<ENTRY>::const_iterator result = codegen_ptr->get_existing_entry_from_symtab(child_node, &symbol_table_instance);
					if(result->type == "FUNCTION")
						if(result->return_type == "INTEGER")
							child_node->set_data_type("INTEGER");
						else
							child_node->set_data_type("REAL");
					else if(result->type == "PROCEDURE")
					{
						codegen_ptr->PrintInstruction(" ");
						child_node->set_data_type("STRING");
					}
					else
						child_node->set_data_type(result->type);
				}
				if(child_node->get_data_type() == "INTEGER")
					codegen_ptr->PrintInstruction("invokevirtual java/io/PrintStream/print(I)V");
				else if(child_node->get_data_type() == "REAL")
					codegen_ptr->PrintInstruction("invokevirtual java/io/PrintStream/print(F)V");
				else
					codegen_ptr->PrintInstruction("invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V");
				node_start->set_is_traversed();
				break;
			}
			case NODE_IDENTIFIER:
			{//For the identifier which is on the RHS
				cout << "NODE_IDENTIFIER : " << node_start->get_id() << endl;
				cout << "current_scope : " << codegen_ptr->get_current_scope() << endl;
				if( ! codegen_ptr->search_from_symbol_table(node_start->get_id(), &symbol_table_instance) )
					node_start = node_start;
				else
				{
					vector<ENTRY>::const_iterator result = codegen_ptr->get_existing_entry_from_symtab(node_start, &symbol_table_instance);
					if(result->type =="FUNCTION")
					{
						string FunctionName(node_start->get_id());
						FUNC_PROC_ITERATOR fp_it = FunctionNameList.find(FunctionName);
						CodeGen_Traversal(node_start->get_leftmost_child(), codegen_ptr);
						codegen_ptr->PrintInstruction("invokestatic " + codegen_ptr->getProgramName() + "/" + (fp_it->first) + (fp_it->second));
						node_start->set_is_traversed();
					}
					else if(result->type=="PROCEDURE")
					{
						string ProcedureName(node_start->get_id());
						FUNC_PROC_ITERATOR fp_it = ProcedureNameList.find(ProcedureName);
						CodeGen_Traversal(node_start->get_leftmost_child(), codegen_ptr);
						codegen_ptr->PrintInstruction("invokestatic " + codegen_ptr->getProgramName() + "+" + (fp_it->first) + (fp_it->second));
						node_start->set_is_traversed();
					}
					else if(result->type == "REAL" || result->type == "INTEGER")
					{
						string type = result->type == "INTEGER" ? "I" : "F";
						if(node_start->get_data_type() == "")
							node_start->set_data_type(result->type);
						if(result->belong_scope == 0)
						{//Global
							codegen_ptr->PrintInstruction("getstatic " + codegen_ptr->getProgramName() + "/" + node_start->get_id() + " " + type);
						}
						else
						{//Local
							int local_serial_number = result->local_serial_number;
							if(type == "I")
								codegen_ptr->PrintInstruction("iload " + numeric2string<int>(local_serial_number));
							else
								codegen_ptr->PrintInstruction("fload " +numeric2string<int>(local_serial_number));
						}
					}
					else if(result->type == "STRING")
					{//String
						if(node_start->get_data_type() == "")
							node_start->set_data_type("STRING");
						if(result->belong_scope == 0)//Global string
							codegen_ptr->PrintInstruction("getstatic " + codegen_ptr->getProgramName() + "/" + node_start->get_id() + " " + "Ljava/lang/String;");
						else//Local string
							codegen_ptr->PrintInstruction("aload " + numeric2string<int>(result->local_serial_number));
					}
					else
					{//Array

					}
				}
				break;
			}
			case NODE_INTEGER:
			{
				//cout << "Inside NODE_INTEGER" << endl;
				int node_value = node_start->get_integer_value();
				string number(numeric2string<int>(node_value));
				if(node_value >= -128 && node_value <= 127)
					codegen_ptr->PrintInstruction("bipush " + number);
				else if(node_value >= -32768 && node_value <= 32767)
					codegen_ptr->PrintInstruction("sipush " + number);
				else
					codegen_ptr->PrintInstruction("ldc " + number);
				break;
			}
			case NODE_REALNUMBER:
			{
				double node_value = node_start->get_real_value();
				string number(numeric2string<double>(node_value));
				codegen_ptr->PrintInstruction("ldc " + number);
				break;
			}
			case NODE_STRING:
			{
				codegen_ptr->PrintInstruction("ldc " + node_start->get_id());
				break;
			}
			case OP_ADD:
			{
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = node_start->get_leftmost_child()->get_rsibling();
				//CodeGen_Traversal(op1, codegen_ptr);
				//CodeGen_Traversal(op2, codegen_ptr);
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					codegen_ptr->PrintInstruction("fadd");
				else
					codegen_ptr->PrintInstruction("iadd");
				node_start->set_is_traversed();
				break;
			}
			case OP_SUB:
			{
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = node_start->get_leftmost_child()->get_rsibling();
				//CodeGen_Traversal(op1, codegen_ptr);
				//CodeGen_Traversal(op2, codegen_ptr);
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					codegen_ptr->PrintInstruction("fsub");
				else
					codegen_ptr->PrintInstruction("isub");
				node_start->set_is_traversed();
				break;
			}
			case OP_MUL:
			{
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = node_start->get_leftmost_child()->get_rsibling();
				//CodeGen_Traversal(op1, codegen_ptr);
				//CodeGen_Traversal(op2, codegen_ptr);
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					codegen_ptr->PrintInstruction("fmul");
				else
					codegen_ptr->PrintInstruction("imul");
				node_start->set_is_traversed();
				break;
			}
			case OP_DIV:
			{
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = node_start->get_leftmost_child()->get_rsibling();
				//CodeGen_Traversal(op1, codegen_ptr);
				//CodeGen_Traversal(op2, codegen_ptr);
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					codegen_ptr->PrintInstruction("fdiv");
				else
					codegen_ptr->PrintInstruction("idiv");
				node_start->set_is_traversed();
				break;
			}
			case OP_LE:
			{//Less or Equal
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = op1->get_rsibling();
				cout << "op1 node type : " << op1->get_node_type() << endl;
				cout << "id : " << op1->get_id() << endl;
				cout << "op2 node type : " << op2->get_node_type() << endl;
				cout << "id : " << op2->get_id() << endl;
				string instrunction2print("");
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					instrunction2print += "fcmpl\n";
				else
					instrunction2print += "isub\n";
				string label_number(numeric2string<int>(if_while_instruction_stack.size()));
				instrunction2print += ("ifgt Lfalse" + label_number);
				if_while_instruction_stack.push_front( deque<string>(1,instrunction2print) );
				if_while_instruction_stack.front().push_back("Lfalse" + label_number + ":");
				if_while_instruction_stack.front().push_back("Lexit" + label_number);
				if_while_instruction_stack.front().push_back("Lexit" + label_number + ":");
				node_start->set_is_traversed();
				break;
			}
			case OP_GE:
			{//Greater or Equal
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = op1->get_rsibling();
				string instrunction2print("");
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					instrunction2print += "fcmpg\n";
				else
					instrunction2print += "isub\n";
				string label_number(numeric2string<int>(if_while_instruction_stack.size()));
				instrunction2print += ("iflt Lfalse" + label_number);
				if_while_instruction_stack.push_front( deque<string> (1, instrunction2print) );
				if_while_instruction_stack.front().push_back("Lfalse" + label_number + ":");
				if_while_instruction_stack.front().push_back("Lexit" + label_number);
				if_while_instruction_stack.front().push_back("Lexit" + label_number + ":");
				node_start->set_is_traversed();
				break;
			}
			case OP_LT:
			{
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = op1->get_rsibling();
				string instrunction2print("");
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					instrunction2print += "fcmpl\n";
				else
					instrunction2print += "isub\n";
				string label_number(numeric2string<int>(if_while_instruction_stack.size()));
				instrunction2print += ("ifge Lfalse" + label_number);
				if_while_instruction_stack.push_front( deque<string>(1, instrunction2print) );
				if_while_instruction_stack.front().push_back("Lfalse" + label_number + ":");
				if_while_instruction_stack.front().push_back("Lexit" + label_number);
				if_while_instruction_stack.front().push_back("Lexit" + label_number + ":");
				node_start->set_is_traversed();
				break;
			}
			case OP_GT:
			{
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = op1->get_rsibling();
				string instrunction2print("");
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					instrunction2print += "fcmpg\n";
				else
					instrunction2print += "isub\n";
				string label_number(numeric2string<int>(if_while_instruction_stack.size()));
				instrunction2print += ("ifle Lfalse" + label_number);
				if_while_instruction_stack.push_front( deque<string>(1, instrunction2print) );
				if_while_instruction_stack.front().push_back("Lfalse" + label_number + ":");
				if_while_instruction_stack.front().push_back("Lexit" + label_number);
				if_while_instruction_stack.front().push_back("Lexit" + label_number + ":");
				node_start->set_is_traversed();
				break;
			}
			case OP_EQUAL:
			{
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = op1->get_rsibling();
				string instrunction2print("");
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					instrunction2print += "fcmpg\n";
				else
					instrunction2print += "isub\n";
				string label_number(numeric2string<int>(if_while_instruction_stack.size()));
				instrunction2print += ("ifne Lfalse" + label_number);
				if_while_instruction_stack.push_front( deque<string>(1, instrunction2print) );
				if_while_instruction_stack.front().push_back("Lfalse" + label_number + ":");
				if_while_instruction_stack.front().push_back("Lexit" + label_number);
				if_while_instruction_stack.front().push_back("Lexit" + label_number + ":");
				node_start->set_is_traversed();
				break;
			}
			case OP_NOTEQUAL:
			{
				Node* op1 = node_start->get_leftmost_child();
				Node* op2 = op1->get_rsibling();
				string instrunction2print("");
				Operand_TypeCasting(op1, codegen_ptr);
				if(op1->get_data_type() == "REAL" || op2->get_data_type() == "REAL")
					instrunction2print += "fcmpg\n";
				else
					instrunction2print += "isub\n";
				string label_number(numeric2string<int>(if_while_instruction_stack.size()));
				instrunction2print += ("ifeq Lfalse" + label_number);
				if_while_instruction_stack.push_front( deque<string>(1, instrunction2print) );
				if_while_instruction_stack.front().push_back("Lfalse" + label_number + ":");
				if_while_instruction_stack.front().push_back("Lexit" + label_number);
				if_while_instruction_stack.front().push_back("Lexit" + label_number + ":");
				node_start->set_is_traversed();
				break;
			}
			default:
				break;
		}
	}
	if(node_start->get_leftmost_child() != NULL && !node_start->get_is_traversed())
		CodeGen_Traversal(node_start->get_leftmost_child(), codegen_ptr);
	if(node_start->get_rsibling() != NULL)
		CodeGen_Traversal(node_start->get_rsibling(), codegen_ptr);
	if(node_start->get_node_type() == NODE_BLOCK && !node_start->get_is_traversed())
	{
		/*Check the block node is not traversed for the reason that we may need to forbid a blcok area to be accessed by turn the is_traversed field as true, so it can't pop any of element from scope_stack_codegen_used stack*/
		codegen_ptr->pop_front_scope_stack_codegen_used();
		scope_codegen_used = codegen_ptr->get_current_scope();
	}
}