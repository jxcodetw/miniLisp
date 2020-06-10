%{
	#include "y.tab.h"
%}
letter [a-z]
digit [0-9]

%%
<<EOF>> {return(0);}
[ \t\n\r] {/*ignore*/}
if {return(IF);}
or {return(OR);}
and {return(AND);}
not {return(NOT);}
mod {return(MOD);}
fun {return(FUN);}
define {return(DEFINE);}
print-num {return(PRINT_NUM);}
print-bool {return(PRINT_BOOL);}
0|[1-9]{digit}*|-[1-9]{digit}* {
    yylval.ival = atoi(yytext);
    return (NUMBER);
}
{letter}({letter}|{digit}|-)* {
    yylval.id = strdup(yytext);
    return (ID);
}
"#t" {yylval.ival=1;return(BOOL_VAL);}
"#f" {yylval.ival=0;return(BOOL_VAL);}
.    { return(yytext[0]);}
%%
