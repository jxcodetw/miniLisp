#ifndef VAR_H
#define VAR_H
#include "astnode.h"
#include <map>

enum VarType {
    V_NULL,
    V_BOOL,
    V_NUMBER,
    V_FUNCTION,
    V_ANY // 代表任意type
};

// 這裡我偷懶 應該要用Polymorphism
// 但我把BOOL, NUMBER, FUNCTION 都用同一個 struct _Var 紀錄
typedef struct _Var {
    enum VarType type;
    // BOOL 跟 NUMBER 都用 val 存值 
    // BOOL 的情況 0: false 1: true
    int val;
    
    // 儲存eval當時的變數列表
    // 可以看testdata/b4_1.lsp
    std::map<std::string, _Var> *env;
    ASTNode *params;
    ASTNode *body;
} Var;

static const Var VARNULL = {
    .type=V_NULL, 
    .val=0
};

// Env 只是 typedef std::map
// 用來儲存變數名稱與值
typedef std::map<std::string, _Var> Env;

#endif
