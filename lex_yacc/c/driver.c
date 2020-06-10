#include <stdio.h>
#include "astnode.h"

int yyparse(void);

void yyerror(const char *msg) {
    printf("%s\n", msg);
    exit(0);
}

int main(int argc, char* argv[]) {
    yyparse();
    evalTree();
    return 0;
}

