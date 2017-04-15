#include "syntax_tree.h"
#include "SDTAction.h"
#include <stdarg.h>
#include <malloc.h>

/* 处理函数定义, 声明和使用 */

ID(6)
{
    if(childNum == 2)
    {
        FuncInfo *funDec = (FuncInfo*)malloc(sizeof(FuncInfo));
        TypeInfo *specifier = (TypeInfo*)(parent->first_child->other_info);
        funDec->return_type = specifier->sType;
        funDec->status = 1;
        funDec->param_num = 0;
        funDec->param_list = NULL;
        child->other_info = funDec;
    }
    else if(childNum == 3)
    {
        TypeInfo *specifier = (TypeInfo*)(parent->first_child->other_info);
        TypeInfo *compSt = (TypeInfo*)malloc(sizeof(TypeInfo));
        compSt->sType = specifier->sType;
        child->other_info = compSt;
    }
}

ID(59)
{
    if(childNum == 2)
    {
        FuncInfo *funDec = (FuncInfo*)malloc(sizeof(FuncInfo));
        TypeInfo *specifier = (TypeInfo*)(parent->first_child->other_info);
        funDec->return_type = specifier->sType;
        funDec->status = 0;
        funDec->param_num = 0;
        funDec->param_list = NULL;
        child->other_info = funDec;
    }
}

ID(18)
{
    if(childNum == 3)
    {
        child->other_info = parent->other_info;
    }
}

ID(20)
{
    if(childNum == 3)
    {
        child->other_info = parent->other_info;
    }
}

ID(22)
{
    if(childNum == 2)
    {
        TypeInfo *varDec = (TypeInfo*)malloc(sizeof(TypeInfo));
        TypeInfo *specifier = (TypeInfo*)(parent->first_child->other_info);
        varDec->iType = specifier->sType;
        varDec->iDimension = 0;
        child->other_info = varDec;
    }
}

ID(23)
{
    if(childNum == 3)
    {
        TypeInfo *stmtList = (TypeInfo*)malloc(sizeof(TypeInfo));
        TypeInfo *compSt = (TypeInfo*)(parent->other_info);
        stmtList->sType = compSt->sType;
        child->other_info = stmtList;
    }
}

ID(24)
{
    if(childNum == 1 || childNum == 2)
    {
        TypeInfo *stmt = (TypeInfo*)malloc(sizeof(TypeInfo));
        TypeInfo *stmtList = (TypeInfo*)(parent->other_info);
        stmt->sType = stmtList->sType;
        child->other_info = stmt;
    }
}

ID(27)
{
    if(childNum == 1)
    {
        TypeInfo *compSt = (TypeInfo*)malloc(sizeof(TypeInfo));
        TypeInfo *stmt = (TypeInfo*)(parent->other_info);
        compSt->sType = stmt->sType;
        child->other_info = compSt;
    }
}

ID(28)
{
    if(childNum == 2)
    {
        TypeInfo *exp = (TypeInfo*)malloc(sizeof(TypeInfo));
        child->other_info = exp;
    }
}

IDS(29, 31)
{
    if(childNum == 3)
    {
        TypeInfo *exp = (TypeInfo*)malloc(sizeof(TypeInfo));
        child->other_info = exp;
    }
    else if(childNum == 5)
    {
        TypeInfo *stmt_ = (TypeInfo*)malloc(sizeof(TypeInfo));
        TypeInfo *stmt = (TypeInfo*)(parent->other_info);
        stmt_->sType = stmt->sType;
        child->other_info = stmt_;
    }
}

ID(30)
{
    if(childNum == 3)
    {
        TypeInfo *exp = (TypeInfo*)malloc(sizeof(TypeInfo));
        child->other_info = exp;
    }
    if(childNum == 5 || childNum == 7)
    {
        TypeInfo *stmt_ = (TypeInfo*)malloc(sizeof(TypeInfo));
        TypeInfo *stmt = (TypeInfo*)(parent->other_info);
        stmt_->sType = stmt->sType;
        child->other_info = stmt_;
    }
}

ID(50)
{
    if(childNum == 3)
    {
        FuncInfo *args = (FuncInfo*)malloc(sizeof(FuncInfo));
        args->param_num = 0;
        args->param_list = NULL;
        child->other_info = args;
        parent->first_child->other_info = args;
    }
}

ID(57)
{
    if(childNum == 1)
    {
        TypeInfo* exp = (TypeInfo*)malloc(sizeof(TypeInfo));
        child->other_info = exp;
    }
    if(childNum == 3)
    {
        child->other_info = parent->other_info;
    }
}

ID(58)
{
    if(childNum == 1)
    {
        TypeInfo* exp = (TypeInfo*)malloc(sizeof(TypeInfo));
        child->other_info = exp; 
    }
}

// SD(9)
// {
//     TypeInfo *specifier = (TypeInfo*)malloc(sizeof(TypeInfo));
//     specifier->sType = parent->first_child->str;
//     parent->other_info = specifier;
// }


SDS(18, 19)
{

    const char *func_name = parent->first_child->str + 4;
    FuncInfo *varList = (FuncInfo*)(parent->other_info);
    int result = addNewFunc(func_name, varList, parent->loc_line);
    parent->other_info = NULL;
    if(result == 0)
        printf("Error type 4 at Line %d: Redefined function \"%s\".\n", 
            parent->loc_line, func_name);
    else if(result == -1)
        printf("Error type 19 at Line %d: Function \"%s\" has been defined with confliction.\n", 
            parent->loc_line, func_name);
    freeTempParamList(varList->param_list);
}

SDS(20, 21)
{
    FuncInfo *varList = (FuncInfo*)(parent->other_info);
    TypeInfo *paramDec = (TypeInfo*)(parent->first_child->other_info);
    if(paramDec == NULL){
        parent->other_info = NULL;
        return;
    }
    AST_node *varDec = parent->first_child->first_child->next_brother;
    while(varDec->first_child != NULL)
        varDec = varDec->first_child;
    // TODO add Struct Type
    addTempFuncParam(varList, varDec->str + 4, paramDec->sType, paramDec->sDimension);
    parent->other_info = NULL;
}

SD(22)
{
    TypeInfo *varDec = (TypeInfo*)(parent->first_child->next_brother->other_info);
    TypeInfo *paramDec = (TypeInfo*)malloc(sizeof(TypeInfo));
    if(varDec->sType != NULL)
    {
        paramDec->sType = varDec->sType;
        paramDec->sDimension = varDec->sDimension;
        parent->other_info = paramDec;
    }
    else
    {
        // 结构体定义非法，不能用该结构体定义变量
        printf("Error type 17 at Line %d: Struct has not been defined.\n", parent->loc_line);
        parent->other_info = NULL;
    }
}

SD(28)
{
    TypeInfo* stmt = (TypeInfo*)(parent->other_info);
    TypeInfo* exp = (TypeInfo*)(parent->first_child->next_brother->other_info);
    if(exp->sValid)
    {
        if(exp->sDimension != 0 || strcmp(exp->sType, stmt->sType) != 0)
            printf("Error type 8 at Line %d: Unmatching return value type.\n", parent->loc_line);
    }

}

SDS(50, 51)
{
    const char* func_name = parent->first_child->str + 4;
    Symbol *symbol_in_table = getFuncSymbol(func_name);
    if(symbol_in_table == NULL)
    {
        if(getSymbol(func_name) != NULL)
            printf("Error type 11 at Line %d: Value %s is not a function.\n",
                parent->loc_line, func_name);
        else
            printf("Error type 2 at Line %d: Function %s has not been defined.\n",
                parent->loc_line, func_name);
        return;
    }
    // 在变量符号表中查找该ID
    // TODO change find symbol function
    FuncInfo *func_call = (FuncInfo*)(parent->first_child->other_info);
    FuncInfo *func_in_table = (FuncInfo*)(symbol_in_table->u.detail);
    if(!checkFuncParamMatch(func_in_table, func_call))
        printf("Error type 9 at Line %d: Function \"%s\" call is not match its defination.\n", 
            parent->loc_line, func_name);
    else
    {
        int old_size = func_in_table->use_line_size;
        func_in_table->use_line = expandFuncUseLine(func_in_table->use_line, old_size);
        func_in_table->use_line[old_size] = -(parent->loc_line);
        func_in_table->use_line_size = old_size + 1;
    }
    freeTempParamList(func_call->param_list);
}


SDS(57, 58)
{
    FuncInfo *args = (FuncInfo*)(parent->other_info);
    TypeInfo *exp = (TypeInfo*)(parent->first_child->other_info);
    if(exp->sValid)
        addTempFuncParam(args, NULL, exp->sType, exp->sDimension);
    parent->other_info = NULL;
}



/**************************************************************/
/**************************************************************/
/* 处理结构体定义和使用 */









void initTable_lxs()
{
    IS(6, 18, 20, 22, 23, 24, 27, 28, 29, 30, 31, 50, 57, 58, 59);
    SS(18, 19, 20, 21, 22, 28, 50, 51, 57, 58);
}