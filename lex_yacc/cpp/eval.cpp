#include <stdio.h>
#include "astnode.h"
#include "var.h"

// 加上static
// root只在這個檔案可見
static ASTNode *root;

// 透過非static的function來指定root
void setRoot(ASTNode *node) {
    root = node;
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

// eval 遞迴的拜訪整顆AST Tree
static Var eval(ASTNode *node, Env &env, enum VarType expected) {
    enum ASTNodeType optype = node->type;
    Var v = VARNULL;
    switch(node->type) {
        /* root */
        case AST_STMTS:
        {
            // 最後一個stmt的回傳值當作整個stmts的回傳值
            // 這樣fun可以在開頭define 然後eval (b3_1.lsp)
            for(int i=0;i<node->len;++i) {
                v = eval(node->child[i], env, V_ANY);
            }
        } break;

        /* basic values */
        case AST_BOOL:   v.type = V_BOOL;   v.val = node->val; break;
        case AST_NUMBER: v.type = V_NUMBER; v.val = node->val; break;
        /* variable */
        case AST_VARIABLE: v = env[node->id]; break;

        /* num-op*/
        case AST_NUMOP:
        {
            // ([+-*/%] exps)
            ASTNode *exps = node->child[0];
            argCheckGreater(exps, 2);

            v = eval(exps->child[0], env, V_NUMBER);
            int tmp = v.val;

            for(int i=1;i<exps->child.size();++i) {
                v = eval(exps->child[i], env, V_NUMBER);
                switch(node->type) {
                    case AST_PLUS:     tmp += v.val; break;
                    case AST_MINUS:    tmp -= v.val; break;
                    case AST_MULTIPLY: tmp *= v.val; break;
                    case AST_DIVIDE:   tmp /= v.val; break;
                    case AST_MODULUS:  tmp %= v.val; break;
                    default: break;
                }
            }

            v.type = V_NUMBER;
            v.val = tmp;
        } break;

        /* cmp */
        case AST_CMP:
        {
            // ([=|>|<] exps)
            node = node->child[0];
            argCheckGreater(node, 2);
            v.type = V_BOOL;
            v.val = 1;

            int target = eval(node->child[0], env, V_NUMBER).val;
            for(int i=1;i<node->len && v.val;++i) {
                int tmp = eval(node->child[i], env, V_NUMBER).val;
                switch(optype) {
                    case AST_GREATER: v.val = (target >  tmp); break;
                    case AST_SMALLER: v.val = (target <  tmp); break;
                    case AST_EQUAL:   v.val = (target == tmp); break;
                    default:break;
                }
            }
        } break;

        /* logical-op */
        case AST_NOT:
        {
            // (not exps)
            node = node->child[0];
            argCheckEqual(node, 1);
            v = eval(node->child[0], env, V_BOOL);
            v.val = !v.val;
        } break;
        case AST_OR:
        case AST_AND:
        {
            // ([and|or] exps)
            node = node->child[0];
            argCheckGreater(node, 2);

            int target = (optype == AST_AND) ? 0 : 1;
            for(int i=0;i<node->len;++i) {
                v = eval(node->child[i], env, V_BOOL);
                if (v.val == target) {
                    break;
                }
            }
        } break;

        /* variable */
        case AST_DEFINE: {
            // (define id val)
            std::string &id = node->child[0]->id;
            v = eval(node->child[1], env, V_ANY);

            if (v.type == V_FUNCTION) {
                // 將自己的function名字也加入function的env snapshot (可以呼叫自己)
                (*v.env)[id] = v;
            }
            env[id] = v;
        } break;

        /* function */
        case AST_FUN:
        {
            // (fun (arg1 arg2 ...) body)
            v.type = V_FUNCTION;
            v.env = new Env(env); // 將目前的env複製一份出來
            v.params = node->child[0];
            v.body = node->child[1];
        } break;
        case AST_FUN_CALL:
        {
            // (funtion args)
            v = eval(node->child[0], env, V_FUNCTION);
            argCheckEqual(node->child[1], v.params->len);

            Env curenv = *v.env;
            for(int i=0;i<v.params->len;++i) {
                curenv[v.params->child[i]->id] = eval(node->child[1]->child[i], env, V_ANY);
            }
            v = eval(v.body, curenv, V_ANY);
        } break;

        /* if */
        case AST_IF:
        {
            // (if test then else)
            // (if exp exp exp)
            v = eval(node->child[0], env, V_BOOL);
            v = eval(node->child[v.val?1:2], env, V_ANY);
        } break;

        /* print */
        case AST_PRINT_NUM:
        {
            v = eval(node->child[0], env, V_NUMBER);
            printf("%d\n", v.val);
        } break;
        case AST_PRINT_BOOL:
        {
            v = eval(node->child[0], env, V_BOOL);
            puts((v.val)?"#t":"#f");
        } break;
        default:
            printf("unhandled ASTNodeType %d\n", node->type);
    }

    // type check
    if (expected != V_ANY && expected != v.type) {
        puts("Type error!");
        exit(0);
    }
    return v;
}

void evalTree() {
    Env env;
    eval(root, env, V_ANY);
}
