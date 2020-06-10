%{
    #include <string>
    #include "astnode.h"
    extern int yylex(void);
    extern void yyerror(const char*);
    extern ASTNode *root;
%}
%error-verbose
%union {
    char* id;
    int ival;
    struct _ASTNode *astnode;
}

%token BOOL_VAL NUMBER ID
%token PRINT_NUM PRINT_BOOL DEFINE MOD AND OR NOT FUN IF
%type<id> ID
%type<ival> NUMBER BOOL_VAL
%type<astnode> stmts stmt variable variables exp exps;

%%
program:stmts{ setRoot($1); }
stmts
: stmt       { $$ = (new ASTNode(AST_STMTS))->addChild($1); }
| stmts stmt { $$ = ($1)->addChild($2); }
stmt: exp
| '(' DEFINE variable exp ')' { $$ = (new ASTNode(AST_DEFINE))->addChild($3)->addChild($4); }
| '(' PRINT_NUM exp ')'       { $$ = (new ASTNode(AST_PRINT_NUM))->addChild($3); }
| '(' PRINT_BOOL exp ')'      { $$ = (new ASTNode(AST_PRINT_BOOL))->addChild($3); }

variables
: /*lambda*/         { $$ = new ASTNode(AST_LIST); }
| variables variable { $$ = ($1)->addChild($2);  }
variable: ID { ($$ = new ASTNode(AST_VARIABLE))->id = $1; }

exps
: /*lambda*/ { $$ = new ASTNode(AST_LIST); }
| exps exp   { $$ = ($1)->addChild($2); }
exp: variable
| BOOL_VAL { ($$ = new ASTNode(AST_BOOL))->val = $1; }
| NUMBER   { ($$ = new ASTNode(AST_NUMBER))->val = $1; }
/* numop */
| '(' '+' exps ')' { $$ = (new ASTNode(AST_PLUS))->addChild($3); }
| '(' '-' exps ')' { $$ = (new ASTNode(AST_MINUS))->addChild($3); }
| '(' '*' exps ')' { $$ = (new ASTNode(AST_MULTIPLY))->addChild($3); }
| '(' '/' exps ')' { $$ = (new ASTNode(AST_DIVIDE))->addChild($3); }
| '(' MOD exps ')' { $$ = (new ASTNode(AST_MODULUS))->addChild($3); }
| '(' '>' exps ')' { $$ = (new ASTNode(AST_GREATER))->addChild($3); }
| '(' '<' exps ')' { $$ = (new ASTNode(AST_SMALLER))->addChild($3); }
| '(' '=' exps ')' { $$ = (new ASTNode(AST_EQUAL))->addChild($3); }
/* logical op */
| '(' AND exps ')' { $$ = (new ASTNode(AST_AND))->addChild($3); }
| '(' OR  exps ')' { $$ = (new ASTNode(AST_OR))->addChild($3); }
| '(' NOT exps ')' { $$ = (new ASTNode(AST_NOT))->addChild($3); }
/* fun funcall if */
| '(' FUN '(' variables ')' stmts ')' { $$ = (new ASTNode(AST_FUN))->addChild($4)->addChild($6); }
| '(' exp exps ')' { $$ = (new ASTNode(AST_FUN_CALL))->addChild($2)->addChild($3); }
| '(' IF exp exp exp ')' { $$ = (new ASTNode(AST_IF))->addChild($3)->addChild($4)->addChild($5); };
%%

