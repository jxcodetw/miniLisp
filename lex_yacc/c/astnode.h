#ifndef ASTNODE_H
#define ASTNODE_H

#include <stdlib.h>
#include <stdarg.h>

enum ASTNodeType {
    AST_STMTS,
    AST_BOOL,
    AST_NUMBER,
    AST_VARIABLE,

    AST_PLUS,
    AST_MINUS,
    AST_MULTIPLY,
    AST_DIVIDE,
    AST_MODULUS,

    AST_GREATER,
    AST_SMALLER,
    AST_EQUAL,

    AST_AND,
    AST_OR,
    AST_NOT,

    AST_DEFINE,
    AST_PARAMS,
    AST_FUN,
    AST_FUN_CALL,
    AST_IF,

    AST_PRINT_NUM,
    AST_PRINT_BOOL,

    AST_LIST
};

struct _ASTNode {
    enum ASTNodeType type;
    int len;
    struct _ASTNode **child;
    
    char *id;
    int val;
};
typedef struct _ASTNode ASTNode;

void evalTree(void);
void setRoot(ASTNode *node);


#endif
