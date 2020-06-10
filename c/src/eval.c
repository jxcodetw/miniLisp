#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "lval.h"
#include "env.h"

extern Builtin builtin;

void typeCheck(lval v, enum L_TYPE type) {
    if (v->type != type) {
#ifdef RICHDEBUG
        printf("expecting: %d, %d given.\n", type, v->type);
#endif
        puts("Type error!");
        exit(-1);
    }
}

static lval lval_symbol(expr e, Env *env) {
    Builtin fb = builtin;
    while(fb && strcmp(e->symbol, fb->name) != 0) {
        fb = fb->next;
    }
    if (fb) {
        lval val = malloc(sizeof(l_val));
        val->type = fb->type;
        val->value = fb->value;
        return val;
    } else {
        Env fe = *env;
        while(fe && strcmp(e->symbol, fe->symbol) != 0) {
            fe = fe->next;
        }
        if (fe) {
            return dup_lval(fe->value);
        } else {
            printf("variable %s not defined\n", e->symbol);
            return NULL;
        }
    }
}

static int is_arithmetic(int value) {
    return value == FUNC_ADD || value == FUNC_SUB ||
        value == FUNC_MUL || value == FUNC_DIV ||
        value == FUNC_MOD;
}

static int is_cmp(int value) {
    return value == FUNC_LT || value == FUNC_GT ||
        value == FUNC_EQ || value == FUNC_LE ||
        value == FUNC_GE;
}

int arg_check(int expected, int given, int mode) {
    if (mode == CHK_EXACT   && given != expected) {
        printf("Error: wrong number of arguments "
            "(expected: %d got: %d)\n", expected, given);
        return 0;
    } else if (mode == CHK_ATLEAST && given <  expected) {
        printf("Error: wrong number of arguments "
            "(at least: %d got: %d)\n", expected, given);
        return 0;
    }
    return 1;
}

static lval call_function(lval func, expr e, Env *env) {
    int i, cmp;
    lval val, tmp;
    if (func->value == FUNC_PRINT) {
        arg_check(1, e->len - 1, CHK_ATLEAST);
        for(i=1;i<e->len;++i) {
            val = eval(e->child[i], env);
            print_lval(val);
        }
        putchar('\n');
        free(func);
        return NULL;
    } else if (func->value == FUNC_NOT) {
        arg_check(1, e->len - 1, CHK_EXACT);
        val = dup_lval(eval(e->child[1], env));
        val->value = !val->value;
        free(func);
        return val;
    } else if (func->value == FUNC_SUB && e->len == 2) {
        val = dup_lval(eval(e->child[1], env));
        val->value = -val->value;
        free(func);
        return val;
    } else if (e->len < 3) {
        return func;
    } else {
        val = eval(e->child[1], env);
        if (is_arithmetic(func->value) || is_cmp(func->value)) {
            typeCheck(val, L_NUMBER);
        }
        tmp = NULL;
        if (is_arithmetic(func->value)) {
            val->type = L_NUMBER;
        } else {
            val->type = L_BOOL;
            cmp = val->value;
            if (!(func->value == FUNC_OR||func->value == FUNC_AND)) {
                val->value = 1;
            }
        }
        for(i=2;i<e->len;++i) {
            free(tmp);
            tmp = eval(e->child[i], env);
            switch(func->value) {
                case FUNC_ADD:
                case FUNC_SUB:
                case FUNC_MUL:
                case FUNC_DIV:
                case FUNC_MOD:
                    typeCheck(val, L_NUMBER);
                    typeCheck(tmp, L_NUMBER);
                    break;
                case FUNC_LT : 
                case FUNC_LE : 
                case FUNC_EQ : 
                case FUNC_GT : 
                case FUNC_GE : 
                    typeCheck(tmp, L_NUMBER);
                    break;
                case FUNC_AND:
                case FUNC_OR:
                    typeCheck(val, L_BOOL);
                    typeCheck(tmp, L_BOOL);
                    break;
            }
            switch(func->value) {
                case FUNC_ADD: val->value += tmp->value; break;
                case FUNC_SUB: val->value -= tmp->value; break;
                case FUNC_MUL: val->value *= tmp->value; break;
                case FUNC_DIV: val->value /= tmp->value; break;
                case FUNC_MOD: val->value %= tmp->value; break;
                case FUNC_LT : val->value &= cmp <  tmp->value; break;
                case FUNC_LE : val->value &= cmp <= tmp->value; break;
                case FUNC_EQ : val->value &= cmp == tmp->value; break;
                case FUNC_GT : val->value &= cmp >  tmp->value; break;
                case FUNC_GE : val->value &= cmp >= tmp->value; break;
                case FUNC_AND: val->value &= tmp->value; break;
                case FUNC_OR : val->value |= tmp->value; break;
            }
        }
        free(tmp);
        free(func);
        return val;
    }
    free(func);
    return NULL;
}

static lval call_syntax(lval func, expr e, Env *env) {
    lval val, choice;
    Env find, env_save;
    switch(func->value) {
        case SYNTAX_IF:
            choice = eval(e->child[1], env);
            if (choice->value) {
                val = eval(e->child[2], env);
            } else {
                val = eval(e->child[3], env);
            }
            free(choice);
            free(func);
            return val;
        case SYNTAX_LET:
            find = *env;
            while(find && strcmp(e->child[1]->symbol, find->symbol)) {
                find=find->next;
            }
            if (find) {
                free(find->value);
                find->value = eval(e->child[2], env);
            } else {
                ext_env(e->child[1]->symbol, eval(e->child[2], env), env);
                find = *env;
            }
            if (find->value->type == L_CLOSURE) {
                ext_env(find->symbol, find->value, &find->value->env);
            }
            free(func);
            return NULL;
        case SYNTAX_LAMBDA:
            free(func);
            return lval_closure(e, *env);
        default:
            printf("undefined syntax %d\n", func->value);
            free(func);
            return NULL;
    }
}

static lval call_closure(lval func, expr e, Env *env) {
    int i;
    lval val, arg;
    expr arglist = func->e->child[1];
    Env old_env = func->env;
    if (arglist->len == e->len-1) {
        for(i=0;i<arglist->len;++i) {
            arg = eval(e->child[i+1], env);
            ext_env(arglist->child[i]->symbol, arg, &func->env);
        }
        val = NULL;
        for(i=2;i<func->e->len;++i) {
            free(val);
            val = eval(func->e->child[i], &func->env);
        }
        free(func);
        return val;
    } else {
        printf("Error: wrong number of arguments "
               "(expected: %d got: %d)\n", arglist->len, e->len-1);
        free(func);
        return NULL;
    }
}

static lval lval_list(expr e, Env *env) {
    lval func = eval(e->child[0], env);
    if (e->len == 1) {
        if (func->type == L_CLOSURE && func->e->child[1]->len == 0) {
            return call_closure(func, e, env);
        }
        return func;
    } else {
        switch(func->type) {
            case L_FUNCTION: return call_function(func, e, env);
            case L_SYNTAX  : return call_syntax(func, e, env);
            case L_CLOSURE : return call_closure(func, e, env);
            default:
                printf("Error: ");
                print_lval(func);
                puts(" is not a function");
                free(func);
                return NULL;
        }
    }
}

lval eval(expr e, Env *env) {
    if (!(e == NULL || (e->type == S_LIST && e->len == 0))) {
        switch(e->type) {
            case S_NUMBER: return lval_number(e->value);
            case S_BOOL  : return lval_bool(e->value);
            case S_SYMBOL: return lval_symbol(e, env);
            case S_LIST  : return lval_list(e, env);
        }
    }
    return NULL;
}
