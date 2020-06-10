%{
    #include "astnode.h"
    #include "astop.h"
    extern int yylex(void);
    extern void yyerror(const char*);
    extern ASTNode *root;
%}
%error-verbose
%union {
    char *id;
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
: stmt       { $$ = addChild(newNode(AST_STMTS), $1); }
| stmts stmt { $$ = addChild($1, $2); }
stmt: exp
| '(' DEFINE variable exp ')' { $$ = addChild(newNode(AST_DEFINE),     $3, $4); }
| '(' PRINT_NUM exp ')'       { $$ = addChild(newNode(AST_PRINT_NUM),  $3); }
| '(' PRINT_BOOL exp ')'      { $$ = addChild(newNode(AST_PRINT_BOOL), $3); }

variables
: /*lambda*/         { $$ = newNode(AST_LIST); }
| variables variable { $$ = addChild($1, $2);  }
variable: ID { ($$ = newNode(AST_VARIABLE))->id = $1; }

exps
: /*lambda*/ { $$ = newNode(AST_LIST); }
| exps exp   { $$ = addChild($1, $2); }
exp: variable
| BOOL_VAL { ($$ = newNode(AST_BOOL))->val = $1; }
| NUMBER   { ($$ = newNode(AST_NUMBER))->val = $1; }
/* numop */
| '(' '+' exps ')' { $$ = addChild(newNode(AST_PLUS),     $3); }
| '(' '-' exps ')' { $$ = addChild(newNode(AST_MINUS),    $3); }
| '(' '*' exps ')' { $$ = addChild(newNode(AST_MULTIPLY), $3); }
| '(' '/' exps ')' { $$ = addChild(newNode(AST_DIVIDE),   $3); }
| '(' MOD exps ')' { $$ = addChild(newNode(AST_MODULUS),  $3); }
| '(' '>' exps ')' { $$ = addChild(newNode(AST_GREATER),  $3); }
| '(' '<' exps ')' { $$ = addChild(newNode(AST_SMALLER),  $3); }
| '(' '=' exps ')' { $$ = addChild(newNode(AST_EQUAL),    $3); }
/* logical op */
| '(' AND exps ')' { $$ = addChild(newNode(AST_AND),      $3); }
| '(' OR  exps ')' { $$ = addChild(newNode(AST_OR),       $3); }
| '(' NOT exps ')' { $$ = addChild(newNode(AST_NOT),      $3); }
/* fun funcall if */
| '(' FUN '(' variables ')' stmts ')' { $$ = addChild(newNode(AST_FUN), $4, $6); }
| '(' exp exps ')' { $$ = addChild(newNode(AST_FUN_CALL), $2, $3); }
| '(' IF exp exp exp ')' { $$ = addChild(newNode(AST_IF), $3, $4, $5); };
%%

