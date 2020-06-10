#include <stdio.h>
#include <string.h>
#include "var.h"
#include "stack.h"

void printEnv(Env **env) {
    Env *head = *env;
    puts("env start");
    while(head != NULL) {
        printf("%s, %d\n", head->id, head->val.val);
        head = head->next;
    }
    puts("env end");
}

void pop(Env **env) {
    if (env == NULL) {
        puts("pop too much");
        exit(0);
    }
    free((*env)->id);
    Env *next = (*env)->next;
    free(*env);
    *env = next;
}

void push(Env **env, char *id, Var val) {
    Env *e = (Env*)malloc(sizeof(Env)), *fe;
    e->id = strdup(id);
    e->val = val;
    e->next = *env;
    *env = e;
    if (val.type == V_FUNCTION) {
        fe = (Env*)malloc(sizeof(Env));
        fe->id = strdup(id);
        fe->val = val;
        fe->next = val.env;
        fe->val.env = fe;
        e->val.env = fe;
    }
}

Var find(Env *env, char *id) {
    Env *head = env;
    while(head != NULL) {
        if (strcmp(head->id, id) == 0) {
            return head->val;
        }
        head = head->next;
    }
    printf("Symbol not defined: %s\n", id);
    exit(0);
    return VARNULL;
}
