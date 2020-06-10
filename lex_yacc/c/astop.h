#ifndef ASTOP_H
#define ASTOP_H

#include "astnode.h"

static inline ASTNode* newNode(enum ASTNodeType type) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->len = 0;
    node->child = NULL;
    node->id = NULL;
    node->val = 0;
    return node;
}

static inline ASTNode* addChild(ASTNode *parent, ...) {
    int num = 1;
    switch(parent->type) {
        case AST_DEFINE:
        case AST_FUN:
        case AST_FUN_CALL: num = 2; break;
        case AST_IF:       num = 3; break;
        default:           num = 1; break;
    }
    va_list valist;
    va_start(valist, parent);
    int t = parent->len;
    
    parent->len += num;
    parent->child=realloc(parent->child, sizeof(ASTNode)*parent->len);
    for(int i=t;i<parent->len;++i) {
        parent->child[i] = va_arg(valist, ASTNode*);
    }

    va_end(valist);
    return parent;
}

#endif
