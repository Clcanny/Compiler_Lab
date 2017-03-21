#include <stdio.h>
#include "syntax.tab.h"
#include "syntax_tree.h"
#include "lex.yy.c"
int main(int argc, char** argv)
{
    if(argc < 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if(!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
//    yylex();
//    fclose(f);
    yyparse();
    fclose(f);
    return 0;
}

yyerror(char *msg)
{
    fprintf(stderr, "error: %s\n", msg);
}
