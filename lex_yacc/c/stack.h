#ifndef STACK_H
#define STACK_H
#include "var.h"

struct _Env {
    char *id;
    Var val;
    struct _Env *next;
};

typedef struct _Env Env;

void printEnv(Env **);
void pop(Env**);
void push(Env**, char *id, Var val);
Var find(Env*, char *id);

#endif
