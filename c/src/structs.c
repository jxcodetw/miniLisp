#include <stdio.h>
#include "structs.h"

static char* function_str[] ={
    "+",
    "-",
    "*",
    "/",
    "mod",

    "<",
    ">",
    "=",
    "<=",
    ">=",

    "and",
    "or",
    "not",

    "print"

};

static char* syntax_str[] ={
    "if",
    "let",
    "lambda"
};

void print_lval(lval val) {
    if (val == NULL) {
        printf("NULL");
    } else {
        switch(val->type) {
            case L_BOOL:
                printf("#%c", val->value ? 't':'f');
                break;
            case L_NUMBER:
                printf("%d", val->value);
                break;
            case L_SYNTAX:
                printf("#<Syntax %s>", syntax_str[val->value]);
                break;
            case L_FUNCTION:
                printf("#<Function %s>", function_str[val->value]);
                break;
            case L_CLOSURE:
                printf("#<Closure>");
                break;
        }
    }
}

void println_lval(lval val) {
    print_lval(val);
    putchar('\n');
}
