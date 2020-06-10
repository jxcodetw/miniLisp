#ifndef VAR_H
#define VAR_H
#include "astnode.h"

enum VarType {
    V_NULL,
    V_BOOL,
    V_NUMBER,
    V_FUNCTION
};

struct _Env;

typedef struct {
    enum VarType type;
    int val;
    
    struct _Env *env;
    ASTNode *params;
    ASTNode *body;
} Var;

static const Var VARNULL = {
    .type=V_NULL, 
    .val=0
};

#endif
