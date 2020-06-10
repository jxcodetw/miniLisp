#ifndef LVAL_H
#define LVAL_H

static lval dup_lval(lval val) {
    if (val == NULL) {
        return NULL;
    }
    lval v = malloc(sizeof(l_val));
    memcpy(v, val, sizeof(l_val));
    return v;
}

static lval lval_number(int value) {
    lval val = malloc(sizeof(l_val));
    val->type = L_NUMBER;
    val->value = value;
    return val;
}

static lval lval_bool(int value) {
    lval val = malloc(sizeof(l_val));
    val->type = L_BOOL;
    val->value = value;
    return val;
}

static lval lval_closure(expr e, Env env) {
    lval val = malloc(sizeof(l_val));
    val->type = L_CLOSURE;
    val->e = e;
    val->env = env;
    return val;
}

#endif
