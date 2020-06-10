#ifndef ASTNODE_H
#define ASTNODE_H

#include <stdlib.h>
#include <stdarg.h>
#include <vector>
#include <string>

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

#define AST_NUMOP AST_PLUS    ... AST_MODULUS
#define AST_CMP   AST_GREATER ... AST_EQUAL

struct _ASTNode {
    enum ASTNodeType type;
    // len可以刪除，用child.size()也是一樣的
    // 只是從舊的code改過來要改太多地方(懶)
    // 所以就保留了
    int len;
    std::vector<struct _ASTNode*> child;
    
    std::string id;
    int val;

    _ASTNode(enum ASTNodeType type) {
        this->type = type;
        this->len = 0;
        this->val = 0;
    }

    _ASTNode* addChild(_ASTNode *child) {
        this->child.push_back(child);
        this->len += 1;
        return this;
    }
};
typedef struct _ASTNode ASTNode;

// 定義在eval.cpp
void evalTree(void);
void setRoot(ASTNode *node);


#endif
