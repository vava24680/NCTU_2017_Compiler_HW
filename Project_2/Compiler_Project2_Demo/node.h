#ifndef __NODE_H__
#define __NODE_H__

struct nodeType;
enum ntype {
    VALUE_INVALID,
    NODE_INT,
    NODE_TERM,
    NODE_PROGRAM,
    NODE_STMT,
    NODE_EXPR,
	NODE_SEMICOLON,           
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
};

// #include "symtab.h"
struct nodeType {
    int nodeType;
    struct nodeType *parent;
    struct nodeType *child;
    struct nodeType *lsibling;
    struct nodeType *rsibling;

    /* Attribute for NODE_TOKEN */
    int tokenType;

    /* items for Array */
    int idxstart;
    int idxend;
    int arraydepth;
    struct nodeType *ref;

    /* Values for general use */
    int iValue;
    double rValue;
    char valueValid;
    char *string;

    /* Indicates which OP */
    char op;

};

struct nodeType* newNode(int type);
void deleteNode(struct nodeType* node);
void addChild(struct nodeType *node, struct nodeType *child);
void printTree(struct nodeType *node, int ident);

#endif

