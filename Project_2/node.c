#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "node.h"
/*extern int ptr_counter;
extern int ptr_array_size;
extern NODE** node_ptr_array;*/

/*NODE** store_into_array(NODE** arr, NODE* node,int* counter,int* size)
{
	int temp = *counter;
	if((*size) == 0)
	{
		(*size) = 64;
		NODE** arr_2  = malloc(sizeof(NODE*) * 64);
		for(int i = 0; i < 64; i++)
		{
			arr_2[i] = NULL;
		}
		arr_2[0] = node;
		arr = arr_2;
	}
	else if((temp+1) == (*size))
	{
		(*size) = (*size) * 2;
		NODE** arr_2 = malloc(sizeof(NODE*) * (*size));
		for(int i = 0;i < (temp+1);i++)
		{
			arr_2[i] = arr[i];
		}
		arr_2[ temp + 1 ] = node;
		for(int i = (temp + 2); i < (*size); i++)
		{
			arr_2[i] = NULL;
		}
		free(arr);
		arr = arr_2;
	}
	else
	{
		arr[temp++] = node;
	}
	*counter = temp;
	return arr;
}*/

NODE* newNode(int type) {
	NODE* node = (struct nodeType*)malloc(sizeof(struct nodeType));
	node->nodeType = type;
	//node->valueValid = VALUE_INVALID;
	node->string = NULL;
	node->parent = NULL;
	node->child = NULL;
	node->lsibling = node;
	node->rsibling = node;
	node->id = NULL;
	node->string = NULL;
	//node_ptr_array = store_into_array(node_ptr_array, node, &ptr_counter, &ptr_array_size);
	return node;
}

void addChild(NODE *node, NODE *child)
{
	child->parent = node;

	if(node->child == NULL)
	{
		node->child = child;
	}
	else
	{
		child->lsibling = node->child->lsibling;
		child->rsibling = node->child;
		node->child->lsibling->rsibling = child;
		node->child->lsibling = child;
	}
}

void deleteNode(NODE *node)
{
	if(node==NULL)
	{
		return;
	}
	while(node->child)
	{
		deleteNode(node->child);
	}
	/*if(node->string != NULL)
		free(node->string);
	free(node);*/
}

void printTree(NODE *node, int ident) {
    static char blank[2048];
    for(int i=0; i<ident; i++)
        blank[i] = '\t';
    blank[ident] = 0;

    switch(node->nodeType) {
		case OP_ADD:
			printf("%s/+\n", blank);
			ident+=1;
			break;
		case OP_SUB:
			printf("%s/-\n", blank);
			ident+=1;
			break;
		case OP_MUL:
			printf("%s/*\n", blank);
			ident+=1;
			break;
		case OP_DIV:
			printf("%s//\n", blank);
			ident+=1;
			break;
		case OP_LT:
			printf("%s/<\n", blank);
			ident+=1;
			break;
		case OP_GT:
			printf("%s/>\n", blank);
			ident+=1;
			break;
		case OP_EQUAL:
			printf("%s/=\n", blank);
			ident+=1;
			break;
		case OP_LE:
			printf("%s/<=\n", blank);
			ident+=1;
			break;
		case OP_GE:
			printf("%s/>=\n", blank);
			ident+=1;
			break;
		case OP_NOTEQUAL:
			printf("%s/!=\n", blank);
			ident+=1;
			break;
		case NODE_ARRAY:
			printf("%s/ARRAY\n", blank);
			ident+=1;
			break;
		case NODE_DO:
			printf("%s/DO\n", blank);
			ident+=1;
			break;
		case NODE_ELSE:
			printf("%s/ELSE\n", blank);
			ident+=1;
			break;
		case NODE_END:
			printf("%s/END\n", blank);
			ident+=1;
			break;
		case NODE_FUNCTION:
			printf("%s/FUNCTION\n", blank);
			ident+=1;
			break;
		case NODE_IF:
			printf("%s/IF\n", blank);
			ident+=1;
			break;
		case NODE_NOT:
			printf("%s/NOT\n", blank);
			ident+=1;
			break;
		case NODE_OF:
			printf("%s/OF\n", blank);
			ident+=1;
			break;
		case NODE_PBEGIN:
			printf("%s/BEGIN\n", blank);
			ident+=1;
			break;
		case NODE_PROCEDURE:
			printf("%s/PROCEDURE\n", blank);
			ident+=1;
			break;
		case NODE_PROGRAM:
			printf("%s/PROGRAM\n", blank);
			ident+=1;
			break;
		case NODE_THEN:
			printf("%s/THEN\n", blank);
			ident+=1;
			break;
		case NODE_VAR:
			printf("%s/VAR\n", blank);
			ident+=1;
			break;
		case NODE_WHILE:
			printf("%s/WHILE\n", blank);
			ident+=1;
			break;
		case NODE_IDENTIFIER:
			printf("%s/IDENTIFIER: %s\n", blank, node->id);
			free(node->id);
			ident+=1;
			break;
		case NODE_ASSIGNMENT:
			printf("%s/ASSIGNMENT\n", blank);
			ident+=1;
			break;
		case NODE_COLON:
			printf("%s/:\n", blank);
			ident+=1;
			break;
		case NODE_COMMA:
			printf("%s/,\n", blank);
			ident+=1;
			break;
		case NODE_INTEGER:
			printf("%s/INTEGER, %d\n", blank, node->integer_value);
			ident+=1;
			break;
		case NODE_DOT:
			printf("%s/.", blank);
			ident+=1;
			break;
		case NODE_DOTDOT:
			printf("%s/..\n", blank);
			ident+=1;
			break;
		case NODE_LBRAC:
			printf("%s/[\n", blank);
			ident+=1;
			break;
		case NODE_LPAREN:
			printf("%s/(\n", blank);
			ident+=1;
			break;
		case NODE_RBRAC:
			printf("%s/]\n", blank);
			ident+=1;
			break;
		case NODE_REALNUMBER:
			printf("%s/REALNUMBER, %f\n", blank, node->real_value);
			ident+=1;
			break;
		case NODE_RPAREN:
			printf("%s/)\n", blank);
			ident+=1;
			break;
		case NODE_SEMICOLON:
			printf("%s/;\n", blank);
			ident+=1;
			break;
		case NODE_NUMBER:
			printf("%s/NUMBER, %s\n", blank, node->string);
			free(node->string);
			ident+=1;
			break;
		case NODE_INT_WORD:
			printf("%s/INTEGER_WORD\n", blank);
			ident+=1;
			break;
		case NODE_REAL_WORD:
			printf("%s/REAL_WORD\n", blank);
			ident+=1;
			break;
		case NODE_STRING_WORD:
			printf("%s/STRING_WORD\n", blank);
			ident+=1;
			break;
		case NODE_SCIENTIFIC_EXP:
			printf("%s/SCIENTIFIC_EXP, %f\n", blank, node->real_value);
			ident+=1;
			break;
		case NODE_SCIENTIFIC_EXP_EXPON:
			printf("%s/SCIENTIFIC_EXP_EXPON\n", blank);
			ident+=1;
			break;
		case NODE_PROG:
			printf("%s/PROG\n", blank);
			ident+=1;
			break;
		case NODE_DECLARATIONS:
			printf("%s/DECLARATIONS\n", blank);
			ident+=1;
			break;
		case NODE_TYPE:
			printf("%s/TYPE\n", blank);
			ident+=1;
			break;
		case NODE_STANDARD_TYPE:
			printf("%s/STANDARD_TYPE\n", blank);
			ident+=1;
			break;
		case NODE_SUBDECLARATIONS:
			printf("%s/SUBDECLARATIONS\n", blank);
			ident+=1;
			break;
		case NODE_SUBDECLARATION:
			printf("%s/SUBDECLARATION\n", blank);
			ident+=1;
			break;
		case NODE_SUBPROGRAM_HEAD:
			printf("%s/SUBPROGRAM_HEAD\n", blank);
			ident+=1;
			break;
		case NODE_ARGUMENTS:
			printf("%s/ARGUMENTS\n", blank);
			ident+=1;
			break;
		case NODE_PARAM_LIST:
			printf("%s/PARAMETER_LIST\n", blank);
			ident+=1;
			break;
		case NODE_OPTIONAL_VAR:
			printf("%s/OPTIONAL VAR\n", blank);
			ident+=1;
			break;
		case NODE_COMP_STATE:
			printf("%s/COMPOUND_STATEMENT\n", blank);
			ident+=1;
			break;
		case NODE_OPTIONAL_STATE:
			printf("%s/OPTIONAL_STATEMENTS\n", blank);
			ident+=1;
			break;
		case NODE_STATEMENT_LIST:
			printf("%s/STATEMENT_LIST\n", blank);
			ident+=1;
			break;
		case NODE_STATEMENT:
			printf("%s/STATEMENT\n", blank);
			ident+=1;
			break;
		case NODE_VARIABLE:
			printf("%s/VARIABLE\n", blank);
			ident+=1;
			break;
		case NODE_TAIL:
			printf("%s/TAIL\n", blank);
			ident+=1;
			break;
		case NODE_PROC_STATE:
			printf("%s/PROCEDURE_STATEMENTS\n", blank);
			ident+=1;
			break;
		case NODE_EXPR_LIST:
			printf("%s/EXPRESSION_LIST\n", blank);
			ident+=1;
			break;
		case NODE_EXPR:
			printf("%s/EXPRESSION\n", blank);
			ident+=1;
			break;
		case NODE_SIM_EXPR:
			printf("%s/SIMPLE_EXPRESSION\n", blank);
			ident+=1;
			break;
		case NODE_TERM:
			printf("%s/TERM\n", blank);
			ident+=1;
			break;
		case NODE_FACTOR:
			printf("%s/FACTOR\n", blank);
			ident+=1;
			break;
		case NODE_ADDOP:
			printf("%s/ADDOP\n", blank);
			ident+=1;
			break;
		case NODE_MULOP:
			printf("%s/MULOP\n", blank);
			ident+=1;
			break;
		case NODE_RELAOP:
			printf("%s/RELAOP\n", blank);
			ident+=1;
			break;
		case NODE_IDENTIFIER_LIST:
			printf("%s/IDENTIFIER_LIST\n", blank);
			ident+=1;
			break;
		case NODE_LAMBDA:
			printf("%s/LAMBDA\n", blank);
			ident+=1;
			break;
        default:
			printf("This is a default value\n");
            printf("%sdefault:%d\n", blank, node->nodeType);
          break;
    }

    NODE *child = node->child;
    if(child != NULL) {
        do {
            printTree(child, ident);
            child = child->rsibling;
        } while(child != node->child);
    }
}

