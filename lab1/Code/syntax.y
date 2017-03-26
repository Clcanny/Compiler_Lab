%locations
%{
    #include "syntax_tree.h"
    extern void yyerror(char *msg);
%}

%union {
    int type_int;
    void* type_node;
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
/* %type <type_node> CompSt StmtList IfMatchedStmt IfOpenStmt OtherStmt Stmt */
%type <type_node> CompSt StmtList Stmt
%type <type_node> DefList Def DecList Dec
/* %type <type_node> Exp Factor MultiplicativeExp AdditiveExp RelationalExp LogicalAndExp LogicalOrExp AssignExp Args */
%type <type_node> Exp Args

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left LP RP LB RB DOT
%right NOT
%left STAR DIV
%left PLUS MINUS
%left RELOP
%left AND
%left OR
%left ASSIGNOP

%%
/* High-level Definitions */
Program 
    : ExtDefList { $$ = new_parent_node("Program", 1, $1); print_child_node($$, 0); }
    ;
ExtDefList
    : ExtDef ExtDefList { $$ = new_parent_node("ExtDefList", 2, $1, $2); }
    | /* empty */ { $$ = new_parent_node("ExtDefList", 0); }
    ;
ExtDef
    : Specifier ExtDecList SEMI { $$ = new_parent_node("ExtDef", 3, $1, $2, $3); }
    | Specifier SEMI { $$ = new_parent_node("ExtDef", 2, $1, $2); }
    | Specifier FunDec CompSt { $$ = new_parent_node("ExtDef", 3, $1, $2, $3); }
    | error SEMI { yyerror("Error ExtDef"); }
    ;
ExtDecList
    : VarDec { $$ = new_parent_node("ExtDecList", 1, $1); }
    | VarDec COMMA ExtDecList { $$ = new_parent_node("ExtDecList", 3, $1, $2, $3); }
    ;

/* Specifiers */
Specifier
    : TYPE { $$ = new_parent_node("Specifier", 1, $1); }
    | StructSpecifier { $$ = new_parent_node("Specifier", 1,$1); }
    ;
StructSpecifier
    : STRUCT OptTag LC DefList RC { $$ = new_parent_node("StructSpecifier", 5, $1, $2, $3, $4, $5); }
    | STRUCT Tag { $$ = new_parent_node("StructSpecifier", 2, $1, $2); }
    ;
OptTag
    :  ID { $$ = new_parent_node("OptTag", 1, $1); }
    | /* empty */ { $$ = new_parent_node("OptTag", 0); }
    ;
Tag
    : ID { $$ = new_parent_node("Tag", 1, $1);}
    ;

/* Declarators */
VarDec
    : ID { $$ = new_parent_node("VarDec", 1, $1); }
    | VarDec LB INT RB { $$ = new_parent_node("varDec", 4, $1, $2, $3, $4); }
    ;
FunDec
    : ID LP VarList RP { $$ = new_parent_node("FunDec", 4, $1, $2, $3, $4); }
    | ID LP RP { $$ = new_parent_node("FunDec", 3, $1, $2, $3); }
    | ID LP error RP { yyerror("Error FunDec"); }
    ;
VarList
    : ParamDec COMMA VarList { $$ = new_parent_node("VarList", 3, $1, $2, $3); }
    | ParamDec { $$ = new_parent_node("VarList", 1, $1); }
    ;
ParamDec
    : Specifier VarDec { $$ = new_parent_node("ParamDec", 2, $1, $2); }
    ;

/* Statements */
CompSt
    : LC DefList StmtList RC { $$ = new_parent_node("CompSt", 4, $1, $2, $3, $4); }
    ;
StmtList
    : Stmt StmtList { $$ = new_parent_node("StmtList", 2, $1, $2); }
    | /* empty */ { $$ = new_parent_node("StmtList", 0); }
    ;
Stmt
    : Exp SEMI { $$ = new_parent_node("Stmt", 2, $1, $2); }
    | CompSt { $$ = new_parent_node("Stmt", 1, $1); }
    | RETURN Exp SEMI { $$ = new_parent_node("Stmt", 3, $1, $2, $3); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = new_parent_node("Stmt", 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt { $$ = new_parent_node("Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
    | WHILE LP Exp RP Stmt { $$ = new_parent_node("Stmt", 5, $1, $2, $3, $4, $5); }
    | error SEMI { yyerror("Error Stmt1"); }
    | IF LP error RP Stmt %prec LOWER_THAN_ELSE { yyerror("Error Stmt2"); }
    | IF LP error RP Stmt ELSE Stmt { yyerror("Error Stmt3"); }
    | WHILE LP error RP Stmt { yyerror("Error Stmt4"); }
    ;
/* OtherStmt */
/*     : Exp SEMI { $$ = new_parent_node("OtherStmt", 2, $1, $2); } */
/*     | CompSt { $$ = new_parent_node("OtherStmt", 1, $1); } */
/*     | RETURN Exp SEMI { $$ = new_parent_node("OtherStmt", 3, $1, $2, $3); } */
/*     | WHILE LP Exp RP Stmt { $$ = new_parent_node("OtherStmt", 5, $1, $2, $3, $4, $5); } */
/*     ; */
/* Stmt */
/*     : IfMatchedStmt { $$ = new_parent_node("Stmt", 1, $1); } */
/*     | IfOpenStmt { $$ = new_parent_node("Stmt", 1, $1); } */
/*     ; */
/* IfMatchedStmt */
/*     : IF LP Exp RP IfMatchedStmt ELSE IfMatchedStmt { $$ = new_parent_node("IfMatchedStmt", 7, $1, $2, $3, $4, $5, $6, $7); } */
/*     | OtherStmt { $$ = new_parent_node("IfMatchedStmt", 1, $1); } */
/*     ; */
/* IfOpenStmt */
/*     : IF LP Exp RP Stmt { $$ = new_parent_node("IfOpenStmt", 5, $1, $2, $3, $4, $5); } */
/*     | IF LP Exp RP IfMatchedStmt ELSE IfOpenStmt { $$ = new_parent_node("IfOpenStmt", 7, $1, $2, $3, $4, $5, $6, $7); } */
/*     ; */

/* Local Definitions */
DefList
    : Def DefList { $$ = new_parent_node("DefList", 2, $1, $2); }
    | /* empty */ { $$ = new_parent_node("DefList", 0); }
    ;
Def
    : Specifier DecList SEMI { $$ = new_parent_node("Def", 3, $1, $2, $3); }
    | error SEMI { yyerror("Error Def"); }
    ;
DecList
    : Dec { $$ = new_parent_node("DecList", 1, $1); }
    | Dec COMMA DecList { $$ = new_parent_node("DecList", 3, $1, $2, $3); }
    ;
Dec
    : VarDec { $$ = new_parent_node("Dec", 1, $1); }
    | VarDec ASSIGNOP Exp { $$ = new_parent_node("Dec", 3, $1, $2, $3); }
    ;

/* Expressions */
Exp
    /* : AssignExp { $$ = new_parent_node("Exp", 1, $1); } */
    : Exp ASSIGNOP Exp {$$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | Exp AND Exp { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | Exp OR Exp { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | Exp RELOP Exp { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | Exp PLUS Exp { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | Exp MINUS Exp { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | Exp STAR Exp { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | Exp DIV Exp { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | LP Exp RP { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | MINUS Exp { $$ = new_parent_node("Exp", 2, $1, $2); }
    | NOT Exp { $$ = new_parent_node("Exp", 2, $1, $2); }
    | ID LP Args RP { $$ = new_parent_node("Exp",4, $1, $2, $3, $4); }
    | ID LP RP { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | Exp LB Exp RB { $$ = new_parent_node("Exp", 4, $1, $2, $3, $4); }
    | Exp DOT ID { $$ = new_parent_node("Exp", 3, $1, $2, $3); }
    | ID { $$ = new_parent_node("Exp", 1, $1); }
    | INT { $$ = new_parent_node("Exp", 1, $1); }
    | FLOAT { $$ = new_parent_node("Exp", 1, $1); }
    ;
/* Factor */
/*     : LP Exp RP { $$ = new_parent_node("Factor", 3, $1, $2, $3); } */
/*     | MINUS Exp { $$ = new_parent_node("Factor", 2, $1, $2); } */
/*     | NOT Exp { $$ = new_parent_node("Factor", 2, $1, $2); } */
/*     | ID LP Args RP { $$ = new_parent_node("Factor", 4, $1, $2, $3, $4); } */
/*     | ID LP RP { $$ = new_parent_node("Factor", 3, $1, $2, $3); } */
/*     | Exp LB Exp RB { $$ = new_parent_node("Factor", 4, $1, $2, $3, $4); } */
/*     | Exp DOT ID { $$ = new_parent_node("Factor", 3, $1, $2, $3); } */
/*     | ID { $$ = new_parent_node("Factor", 1, $1); } */
/*     | INT { $$ = new_parent_node("Factor", 1, $1); } */
/*     | FLOAT { $$ = new_parent_node("Factor", 1, $1); } */
/*     ; */
/* MultiplicativeExp */
/*     : MultiplicativeExp STAR Factor { $$ = new_parent_node("MultiplicativeExp", 3, $1, $2, $3); } */
/*     | MultiplicativeExp DIV Factor { $$ = new_parent_node("MultiplicativeExp", 3, $1, $2, $3); } */
/*     | Factor { $$ = new_parent_node("MultiplicativeExp", 1, $1); } */
/*     ; */
/* AdditiveExp */
/*     : AdditiveExp PLUS MultiplicativeExp { $$ = new_parent_node("AdditiveExp", 3, $1, $2, $3); } */
/*     | AdditiveExp MINUS MultiplicativeExp { $$ = new_parent_node("AdditiveExp", 3, $1, $2, $3); } */
/*     | MultiplicativeExp { $$ = new_parent_node("AdditiveExp", 1, $1); } */
/*     ; */
/* RelationalExp */
/*     : RelationalExp RELOP AdditiveExp { $$ = new_parent_node("RelationalExp", 3, $1, $2, $3); } */
/*     | AdditiveExp { $$ = new_parent_node("RelationalExp", 1, $1); } */
/*     ; */
/* LogicalAndExp */
/*     : LogicalAndExp AND RelationalExp { $$ = new_parent_node("LogicalAndExp", 3, $1, $2, $3); } */
/*     | RelationalExp { $$ = new_parent_node("LogicalAndExp", 1, $1); } */
/*     ; */
/* LogicalOrExp */
/*     : LogicalOrExp OR LogicalAndExp { $$ = new_parent_node("LogicalOrExp", 3, $1, $2, $3); } */
/*     | LogicalAndExp { $$ = new_parent_node("LogicalOrExp", 1, $1); } */
/*     ; */
/* AssignExp */
/*     : LogicalOrExp ASSIGNOP AssignExp { $$ = new_parent_node("AssignExp", 3, $1, $2, $3); } */
/*     | LogicalOrExp { $$ = new_parent_node("AssignExp", 1, $1); } */
/*     ; */

Args
    : Exp COMMA Args { $$ = new_parent_node("Args", 3, $1, $2, $3); }
    | Exp { $$ = new_parent_node("Args", 1, $1); }
    ;
