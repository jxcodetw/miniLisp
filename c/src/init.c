#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "structs.h"

Builtin builtin;

static void add_builtin(char* name, int type, int value) {
    Builtin b = malloc(sizeof(struct Builtin));
    b->name  = name;
    b->type  = type;
    b->value = value;
    b->next  = builtin;
    builtin  = b;
}

void init_builtin() {
    add_builtin("+",      L_FUNCTION, FUNC_ADD);
    add_builtin("-",      L_FUNCTION, FUNC_SUB);
    add_builtin("*",      L_FUNCTION, FUNC_MUL);
    add_builtin("/",      L_FUNCTION, FUNC_DIV);
    add_builtin("%",      L_FUNCTION, FUNC_MOD);
    add_builtin("mod",    L_FUNCTION, FUNC_MOD);
    add_builtin("<",      L_FUNCTION, FUNC_LT);
    add_builtin("<=",     L_FUNCTION, FUNC_LE);
    add_builtin("=",      L_FUNCTION, FUNC_EQ);
    add_builtin(">",      L_FUNCTION, FUNC_GT);
    add_builtin(">=",     L_FUNCTION, FUNC_GE);
    add_builtin("or",     L_FUNCTION, FUNC_OR);
    add_builtin("and",    L_FUNCTION, FUNC_AND);
    add_builtin("not",    L_FUNCTION, FUNC_NOT);
    add_builtin("print",  L_FUNCTION, FUNC_PRINT);
    add_builtin("print-num",  L_FUNCTION, FUNC_PRINT);
    add_builtin("print-bool",  L_FUNCTION, FUNC_PRINT);
    add_builtin("if",     L_SYNTAX  , SYNTAX_IF);
    add_builtin("let",    L_SYNTAX  , SYNTAX_LET);
    add_builtin("define", L_SYNTAX  , SYNTAX_LET);
    add_builtin("fun",    L_SYNTAX  , SYNTAX_LAMBDA);
    add_builtin("lambda", L_SYNTAX  , SYNTAX_LAMBDA);
}

regex_t preg_int;
regmatch_t pmatch;

void init_regex() {
    if (regcomp(&preg_int, "[-+]?[0-9]+", REG_EXTENDED | REG_ICASE)) {
        puts("regex fail");
    }
}
