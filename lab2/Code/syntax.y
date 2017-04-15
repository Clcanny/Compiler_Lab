%locations
%define parse.error verbose
%{
    #include "syntax_tree.h"
    #include "SDTAction.h"
    #include "symbol_table.h"
    int has_error = 0;
    void yyerror(const char *msg);
    void yyerror_lineno(const char *msg, int lineno);
%}

%union {
    int type_int;
    void *type_node;
}

%token <type_node> ID
%token <type_node> ASSIGNOP RELOP AND OR NOT
%token <type_node> PLUS MINUS STAR DIV
%token <type_node> TYPE STRUCT INT FLOAT
%token <type_node> IF ELSE WHILE RETURN
%token <type_node> SEMI COMMA DOT
%token <type_node> LP RP LB RB LC RC

%type <type_node> Program ExtDefList ExtDef ExtDecList
%type <type_node> Specifier StructSpecifier OptTag Tag
%type <type_node> VarDec FunDec VarList ParamDec
%type <type_node> CompSt StmtList Stmt
%type <type_node> DefList Def DecList Dec
%type <type_node> Exp Args

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%%
/* High-level Definitions */
Program
    : ExtDefList {
        $$ = new_parent_node("Program", 1, 1, $1);
        /* print_child_node($$, 0); */
        initTable();
        initTable_lxs();
        globalSymbolTable = newSymbolTable();
        globalFuncSymbolTable = newFuncSymbolTable();
        traversalTreePerformAction($$);
        findUndefinedFunction();
        clean_up_syntax_tree($$);
        cleanUpSymbolTable();
    }
    ;
ExtDefList
    : ExtDef ExtDefList { $$ = new_parent_node("ExtDefList", 2, 2, $1, $2); }
    | /* empty */ { $$ = new_parent_node("EMPTY", 3, 0); }
    ;
ExtDef
    : Specifier ExtDecList SEMI { $$ = new_parent_node("ExtDef", 3, 4, $1, $2, $3); }
    | Specifier SEMI { $$ = new_parent_node("ExtDef", 5, 2, $1, $2); }
    | Specifier FunDec CompSt { $$ = new_parent_node("ExtDef", 6, 3, $1, $2, $3); }
    | Specifier FunDec SEMI { $$ = new_parent_node("ExtDef", 59, 3, $1, $2, $3); }
    ;
ExtDecList
    : VarDec { $$ = new_parent_node("ExtDecList", 7, 1, $1); }
    | VarDec COMMA ExtDecList { $$ = new_parent_node("ExtDecList", 8, 3, $1, $2, $3); }
    ;

/* Specifiers */
Specifier
    : TYPE { $$ = new_parent_node("Specifier", 9, 1, $1); }
    | StructSpecifier { $$ = new_parent_node("Specifier", 10, 1,$1); }
    ;
StructSpecifier
    : STRUCT OptTag LC DefList RC { $$ = new_parent_node("StructSpecifier", 11, 5, $1, $2, $3, $4, $5); }
    | STRUCT Tag { $$ = new_parent_node("StructSpecifier", 12, 2, $1, $2); }
    ;
OptTag
    :  ID { $$ = new_parent_node("OptTag", 13, 1, $1); }
    | /* empty */ { $$ = new_parent_node("EMPTY", 14, 0); }
    ;
Tag
    : ID { $$ = new_parent_node("Tag", 15, 1, $1);}
    ;

/* Declarators */
VarDec
    : ID { $$ = new_parent_node("VarDec", 16, 1, $1); }
    | VarDec LB INT RB { $$ = new_parent_node("varDec", 17, 4, $1, $2, $3, $4); }
    ;
FunDec
    : ID LP VarList RP { $$ = new_parent_node("FunDec", 18, 4, $1, $2, $3, $4); }
    | ID LP RP { $$ = new_parent_node("FunDec", 19, 3, $1, $2, $3); }
    ;
VarList
    : ParamDec COMMA VarList { $$ = new_parent_node("VarList", 20, 3, $1, $2, $3); }
    | ParamDec { $$ = new_parent_node("VarList", 21, 1, $1); }
    ;
ParamDec
    : Specifier VarDec { $$ = new_parent_node("ParamDec", 22, 2, $1, $2); }
    ;

/* Statements */
CompSt
    : LC DefList StmtList RC { $$ = new_parent_node("CompSt", 23, 4, $1, $2, $3, $4); }
    ;

StmtList
    : Stmt StmtList { $$ = new_parent_node("StmtList", 24, 2, $1, $2); }
    | /* empty */ { $$ = new_parent_node("EMPTY", 25, 0); }
    ;
Stmt
    : Exp SEMI { $$ = new_parent_node("Stmt", 26, 2, $1, $2); }
    | CompSt { $$ = new_parent_node("Stmt", 27, 1, $1); }
    | RETURN Exp SEMI { $$ = new_parent_node("Stmt", 28, 3, $1, $2, $3); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = new_parent_node("Stmt", 29, 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt { $$ = new_parent_node("Stmt", 30, 7, $1, $2, $3, $4, $5, $6, $7); }
    | WHILE LP Exp RP Stmt { $$ = new_parent_node("Stmt", 31, 5, $1, $2, $3, $4, $5); }
    ;

/* Local Definitions */
DefList
    : Def DefList { $$ = new_parent_node("DefList", 32, 2, $1, $2); }
    | /* empty */ { $$ = new_parent_node("EMPTY", 33, 0); }
    ;
Def
    : Specifier DecList SEMI { $$ = new_parent_node("Def", 34, 3, $1, $2, $3); }
    ;
DecList
    : Dec { $$ = new_parent_node("DecList", 35, 1, $1); }
    | Dec COMMA DecList { $$ = new_parent_node("DecList", 36, 3, $1, $2, $3); }
    ;
Dec
    : VarDec { $$ = new_parent_node("Dec", 37, 1, $1); }
    | VarDec ASSIGNOP Exp { $$ = new_parent_node("Dec", 38, 3, $1, $2, $3); }
    ;

/* Expressions */
Exp
    : Exp ASSIGNOP Exp {$$ = new_parent_node("Exp", 39, 3, $1, $2, $3); }
    | Exp AND Exp { $$ = new_parent_node("Exp", 40, 3, $1, $2, $3); }
    | Exp OR Exp { $$ = new_parent_node("Exp", 41, 3, $1, $2, $3); }
    | Exp RELOP Exp { $$ = new_parent_node("Exp", 42, 3, $1, $2, $3); }
    | Exp PLUS Exp { $$ = new_parent_node("Exp", 43, 3, $1, $2, $3); }
    | Exp MINUS Exp { $$ = new_parent_node("Exp", 44, 3, $1, $2, $3); }
    | Exp STAR Exp { $$ = new_parent_node("Exp", 45, 3, $1, $2, $3); }
    | Exp DIV Exp { $$ = new_parent_node("Exp", 46, 3, $1, $2, $3); }
    | LP Exp RP { $$ = new_parent_node("Exp", 47, 3, $1, $2, $3); }
    | MINUS Exp { $$ = new_parent_node("Exp", 48, 2, $1, $2); }
    | NOT Exp { $$ = new_parent_node("Exp", 49, 2, $1, $2); }
    | ID LP Args RP { $$ = new_parent_node("Exp", 50, 4, $1, $2, $3, $4); }
    | ID LP RP { $$ = new_parent_node("Exp", 51, 3, $1, $2, $3); }
    | Exp LB Exp RB { $$ = new_parent_node("Exp", 52, 4, $1, $2, $3, $4); }
    | Exp DOT ID { $$ = new_parent_node("Exp", 53, 3, $1, $2, $3); }
    | ID { $$ = new_parent_node("Exp", 54, 1, $1); }
    | INT { $$ = new_parent_node("Exp", 55, 1, $1); }
    | FLOAT { $$ = new_parent_node("Exp", 56, 1, $1); }
    ;

Args
    : Exp COMMA Args { $$ = new_parent_node("Args", 57, 3, $1, $2, $3); }
    | Exp { $$ = new_parent_node("Args", 58, 1, $1); }
    ;
