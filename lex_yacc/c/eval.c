#include <stdio.h>
#include "astnode.h"
#include "var.h"
#include "stack.h"

static void typeCheck(Var *v, enum VarType expected) {
    if (expected != v->type) {
        //printf("e%d, g%d\n", expected, v->type);
        puts("Type error!");
        exit(0);
    }
}

static void argCheckEqual(ASTNode *node, int expected) {
    if (node->len != expected) {
        printf("Need %d arguments, but got %d.\n", expected, node->len);
        exit(0);
    }
}

static void argCheckGreater(ASTNode *node, int expected) {
    if (node->len < expected) {
        printf("Need %d arguments, but got %d. \n", expected, node->len);
        exit(0);
    }
}

static void printASTNodeType(ASTNode *node) {
    switch(node->type) {
        case AST_STMTS: puts("AST_STMTS"); break;
        case AST_BOOL: puts("AST_BOOL"); break;
        case AST_NUMBER: puts("AST_NUMBER"); break;
        case AST_VARIABLE: puts("AST_VARIABLE"); break;
        case AST_PLUS: puts("AST_PLUS"); break;
        case AST_MINUS: puts("AST_MINUS"); break;
        case AST_MULTIPLY: puts("AST_MULTIPLY"); break;
        case AST_DIVIDE: puts("AST_DIVIDE"); break;
        case AST_MODULUS: puts("AST_MODULUS"); break;
        case AST_GREATER: puts("AST_GREATER"); break;
        case AST_SMALLER: puts("AST_SMALLER"); break;
        case AST_EQUAL: puts("AST_EQUAL"); break;
        case AST_AND: puts("AST_AND"); break;
        case AST_OR: puts("AST_OR"); break;
        case AST_NOT: puts("AST_NOT"); break;
        case AST_DEFINE: puts("AST_DEFINE"); break;
        case AST_PARAMS: puts("AST_PARAMS"); break;
        case AST_FUN: puts("AST_FUN"); break;
        case AST_FUN_CALL: puts("AST_FUN_CALL"); break;
        case AST_IF: puts("AST_IF"); break;
        case AST_PRINT_NUM: puts("AST_PRINT_NUM"); break;
        case AST_PRINT_BOOL: puts("AST_PRINT_BOOL"); break;

        case AST_LIST: puts("AST_LIST"); break;
    }
}

static ASTNode *root;

void setRoot(ASTNode *node) {
    root = node;
}

static Var eval(ASTNode *node, Env **env) {
    Env *curenv = *env;
    Var v = VARNULL, test;
    int i, tmp;
    switch(node->type) {
        /* root */
        case AST_STMTS:
            for(i=0;i<node->len;++i) {
                v = eval(node->child[i], env);
            }
            break;

        /* basic values */
        case AST_BOOL:
            v.type = V_BOOL;
            v.val = node->val;
            break;
        case AST_NUMBER:
            v.type = V_NUMBER;
            v.val = node->val;
            break;

        /* num-op*/
        case AST_PLUS:
            node = node->child[0];
            argCheckGreater(node, 2);
            tmp = 0;
            for(i=0;i<node->len;++i) {
                v = eval(node->child[i], env);
                typeCheck(&v, V_NUMBER);
                tmp += v.val;
            }
            v.type = V_NUMBER;
            v.val = tmp;
            break;
        case AST_MINUS:
            // exact two arg implementation
            node = node->child[0];
            argCheckEqual(node, 2);
            tmp = 0;
            v = eval(node->child[0], env);
            typeCheck(&v, V_NUMBER);
            tmp = v.val;

            v = eval(node->child[1], env);
            typeCheck(&v, V_NUMBER);
            tmp -= v.val;
            
            v.type = V_NUMBER;
            v.val = tmp;
            break;
        case AST_MULTIPLY:
            node = node->child[0];
            argCheckGreater(node, 2);
            tmp = 1;
            for(i=0;i<node->len;++i) {
                v = eval(node->child[i], env);
                typeCheck(&v, V_NUMBER);
                tmp *= v.val;
            }
            v.type = V_NUMBER;
            v.val = tmp;
            break;
        case AST_DIVIDE:
            // exact two arg implementation
            node = node->child[0];
            argCheckEqual(node, 2);
            tmp = 0;
            v = eval(node->child[0], env);
            typeCheck(&v, V_NUMBER);
            tmp = v.val;

            v = eval(node->child[1], env);
            typeCheck(&v, V_NUMBER);
            tmp /= v.val;
            
            v.type = V_NUMBER;
            v.val = tmp;
            break;
        case AST_MODULUS:
            // exact two arg implementation
            node = node->child[0];
            argCheckEqual(node, 2);
            tmp = 0;
            v = eval(node->child[0], env);
            typeCheck(&v, V_NUMBER);
            tmp = v.val;

            v = eval(node->child[1], env);
            typeCheck(&v, V_NUMBER);
            tmp = tmp % v.val;
            
            v.type = V_NUMBER;
            v.val = tmp;
            break;

        /* cmp */
        case AST_GREATER:
            // exact two arg implementation
            node = node->child[0];
            argCheckEqual(node, 2);
            tmp = 0;
            v = eval(node->child[0], env);
            typeCheck(&v, V_NUMBER);
            tmp = v.val;

            v = eval(node->child[1], env);
            typeCheck(&v, V_NUMBER);
            tmp = tmp > v.val;
            
            v.type = V_BOOL;
            v.val = tmp;
            break;
        case AST_SMALLER:
            // exact two arg implementation
            node = node->child[0];
            argCheckEqual(node, 2);
            tmp = 0;
            v = eval(node->child[0], env);
            typeCheck(&v, V_NUMBER);
            tmp = v.val;

            v = eval(node->child[1], env);
            typeCheck(&v, V_NUMBER);
            tmp = tmp < v.val;
            
            v.type = V_BOOL;
            v.val = tmp;
            break;
        case AST_EQUAL:
            node = node->child[0];
            argCheckGreater(node, 2);
            v = eval(node->child[0], env);
            typeCheck(&v, V_NUMBER);
            tmp = v.val;
            for(i=1;i<node->len;++i) {
                v = eval(node->child[i], env);
                typeCheck(&v, V_NUMBER);
                if (tmp != v.val) {
                    break;
                }
            }
            v.type = V_BOOL;
            v.val = (tmp == v.val);
            break;

        /* logical-op */
        case AST_NOT:
            node = node->child[0];
            argCheckEqual(node, 1);
            v = eval(node->child[0], env);
            typeCheck(&v, V_BOOL);
            v.val = !v.val;
            break;
        case AST_AND:
            node = node->child[0];
            argCheckGreater(node, 2);
            tmp = 1;
            for(int i=0;i<node->len && tmp;++i) {
                v = eval(node->child[i], env);
                typeCheck(&v, V_BOOL);
                tmp = tmp && v.val;
            } 
            v.type = V_BOOL;
            v.val = tmp;
            break;
        case AST_OR:
            node = node->child[0];
            argCheckGreater(node, 2);
            tmp = 0;
            for(int i=0;i<node->len && tmp == 0;++i) {
                v = eval(node->child[i], env);
                typeCheck(&v, V_BOOL);
                tmp = tmp || v.val;
            } 
            v.type = V_BOOL;
            v.val = tmp;
            break;

        /* variable */
        case AST_VARIABLE:
            v = find(*env, node->id);
            break;
        case AST_DEFINE:
            push(env, node->child[0]->id, eval(node->child[1], env));
            break;

        /* function */
        case AST_FUN:
            v.type = V_FUNCTION;
            v.env = *env;
            v.params = node->child[0];
            v.body = node->child[1];
            break;
        case AST_FUN_CALL:
            v = eval(node->child[0], env);
            typeCheck(&v, V_FUNCTION);
            argCheckEqual(node->child[1], v.params->len);
            tmp = v.params->len;
            curenv = v.env;
            for(i=0;i<tmp;++i) {
                test = eval(node->child[1]->child[i], env);
                push(&curenv, v.params->child[i]->id, test);
            }
            v = eval(v.body, &curenv);
            break;

        /* if */
        case AST_IF:
            v = eval(node->child[0], env);
            typeCheck(&v, V_BOOL);
            if (v.val) {
                v = eval(node->child[1], env);
            } else {
                v = eval(node->child[2], env);
            }
            break;

        /* print */
        case AST_PRINT_NUM:
            v = eval(node->child[0], env);
            typeCheck(&v, V_NUMBER);
            printf("%d\n", v.val);
            break;
        case AST_PRINT_BOOL:
            v = eval(node->child[0], env);
            typeCheck(&v, V_BOOL);
            puts((v.val)?"#t":"#f");
            break;
        default:
            printf("unhandled ASTNodeType %d\n", node->type);
    }
    return v;
}

void evalTree() {
    Env *env = NULL;
    eval(root, &env);
}
