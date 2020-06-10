#ifndef STRUCTS_H
#define STRUCTS_H

enum ARG_CHECK {CHK_EXACT, CHK_ATLEAST};
enum S_TYPE {S_SYMBOL, S_LIST, S_NUMBER, S_BOOL};
enum L_TYPE {L_NUMBER, L_FUNCTION, L_BOOL, L_SYNTAX, L_CLOSURE};
enum FUNC_TYPE {
    FUNC_ADD,
    FUNC_SUB,
    FUNC_MUL,
    FUNC_DIV,
    FUNC_MOD,

    FUNC_LT, // less than
    FUNC_GT, // greater than
    FUNC_EQ,
    FUNC_LE,
    FUNC_GE,

    FUNC_AND,
    FUNC_OR,
    FUNC_NOT,

    FUNC_PRINT
};

enum SYNTAX_TYPE {
    SYNTAX_IF,
    SYNTAX_LET,
    SYNTAX_LAMBDA
};

typedef struct lval* lval;

typedef struct s_expr {
    int type;
    int len;
    int value;
    char* symbol;
    struct s_expr** child;
} s_expr, *expr;

typedef struct Env {
    lval value;
    char* symbol;
    struct Env* next;
} *Env;

typedef struct lval {
    int type;
    int value;
    char* symbol;
    expr e;
    Env env;
} l_val, *lval;

typedef struct Builtin {
    char* name;
    int type;
    int value;
    struct Builtin* next;
} *Builtin;

// init.c
void init_regex();
void init_builtin();
// structs.c
void print_lval(lval val);
void println_lval(lval val);
lval eval(expr e, Env *env);
// parser.c
expr parse(char* s, char** e);

#endif
