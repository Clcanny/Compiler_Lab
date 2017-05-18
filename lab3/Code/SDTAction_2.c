#include "syntax_tree.h"
#include "SDTAction.h"
#include <stdarg.h>
#include <malloc.h>

/* 处理函数定义, 声明和使用 */

ID(6)
{
    if(childNum == 2)
    {
        const char *return_type;
        TypeInfo *specifier = (TypeInfo*)(parent->first_child->other_info);
        if(!specifier->sValid)
            return_type = NULL;
        else
            return_type = specifier->sType;
        startDefineFunction(child->first_child->str + 4, 1, return_type, 
            child->first_child);
    }
    else if(childNum == 3)
    {
        TypeInfo *specifier = (TypeInfo*)(parent->first_child->other_info);
        TypeInfo *compSt = (TypeInfo*)malloc(sizeof(TypeInfo));
        if(!specifier->sValid)
            compSt->sType = NULL;
        else
            compSt->sType = specifier->sType;
        compSt->sValid = 1;
        child->other_info = compSt;

// IR Generation
        AST_node *func_id = parent->first_child->next_brother->first_child;
        func_id->IRIndex = nextFuncIndex++;
        FuncInfo *function = findFuncSymbol(func_id->str + 4);
        gen_IR(Fun, new_value(F, func_id->IRIndex));
        Symbol *param = function->param_list;
        while(param != NULL) 
        {
            ((AST_node*)(param->p))->IRIndex = nextVarIndex++;
            gen_IR(Param, new_value(V, nextVarIndex - 1));
            param = param->next;
        }
// end
    }
}

ID(59)
{
    if(childNum == 2)
    {
        const char *return_type;
        TypeInfo *specifier = (TypeInfo*)(parent->first_child->other_info);
        if(!specifier->sValid)
            return_type = NULL;
        else
            return_type = specifier->sType;
        startDefineFunction(child->first_child->str + 4, 0, return_type, NULL);
    }
}

ID(18)
{
    // if(childNum == 3)
    // {
    //     child->other_info = parent->other_info;
    // }
}

ID(20)
{
    // if(childNum == 3)
    // {
    //     child->other_info = parent->other_info;
    // }
}

ID(22)
{
    if(childNum == 2)
    {
        TypeInfo *varDec = (TypeInfo*)malloc(sizeof(TypeInfo));
        TypeInfo *specifier = (TypeInfo*)(parent->first_child->other_info);
        varDec->iType = specifier->sType;
        varDec->iDimension = 0;
        varDec->sValid = specifier->sValid;
        child->other_info = varDec;
    }
}


SDS(18, 19)
{
    const char *func_name = parent->first_child->str + 4;
    int result = funcDefineEnd(parent->loc_line);
    if(result == 0)
        printf("Error type 4 at Line %d: Redefined function \"%s\".\n", 
            parent->loc_line, func_name);
    else if(result == -1)
        printf("Error type 19 at Line %d: Function \"%s\" has been defined with confliction.\n", 
            parent->loc_line, func_name);
}

SDS(20, 21)
{
    // FuncInfo *varList = (FuncInfo*)(parent->other_info);
    // TypeInfo *paramDec = (TypeInfo*)(parent->first_child->other_info);
    // if(paramDec == NULL){
    //     parent->other_info = NULL;
    //     return;
    // }
    // AST_node *varDec = parent->first_child->first_child->next_brother;
    // while(varDec->first_child != NULL)
    //     varDec = varDec->first_child;
    // // TODO add Struct Type
    // addTempFuncParam(varList, varDec->str + 4, paramDec->sType, paramDec->sDimension);
    // parent->other_info = NULL;
}

SD(22)
{
    // TypeInfo *varDec = (TypeInfo*)(parent->first_child->next_brother->other_info);
    // TypeInfo *paramDec = (TypeInfo*)malloc(sizeof(TypeInfo));
    // if(varDec->sType != NULL)
    // {
    //     paramDec->sType = varDec->sType;
    //     paramDec->sDimension = varDec->sDimension;
    //     paramDec->sValid = varDec->sValid;
    //     parent->other_info = paramDec;
    // }
    // else
    // {
    //     // 结构体定义非法，不能用该结构体定义变量
    //     printf("Error type 17 at Line %d: Struct has not been defined.\n", parent->loc_line);
    //     parent->other_info = NULL;
    // }
}


/**************************************************************/
/**************************************************************/
/* 处理函数调用 */


ID(50)
{
    if(childNum == 0)
    {
        parent->IRIndex = nextVarIndex++;
    }

    if(childNum == 3)
    {
        FuncInfo *args = (FuncInfo*)malloc(sizeof(FuncInfo));
        args->name = parent->first_child->str + 4;
        args->param_num = 0;
        args->param_list = NULL;
        child->other_info = args;
        parent->first_child->other_info = args;

// IR Generation

    }
}

ID(51)
{
    if(childNum == 0)
    {
        parent->IRIndex = nextVarIndex++;
    }

    if(childNum == 2)
    {
        FuncInfo *args = (FuncInfo*)malloc(sizeof(FuncInfo));
        args->name = parent->first_child->str + 4;
        args->param_num = 0;
        args->param_list = NULL;
        parent->first_child->other_info = args; 

// IR Generation
        char *func_id = parent->first_child->str + 4;
        AST_node *function = (AST_node*)(findFuncSymbol(func_id)->p);
        int place_index = parent->IRIndex;
        if(strcmp(func_id, "read") == 0)
            gen_IR(Read, new_value(V, place_index));
        else
            gen_IR(Call, new_value(V, place_index), new_value(F, function->IRIndex));
// end
    }
}

ID(57)
{
    if(childNum == 1)
    {
        TypeInfo* exp = (TypeInfo*)malloc(sizeof(TypeInfo));
        exp->iDimension = 0;
        child->other_info = exp;
        child->IRIndex = nextVarIndex++;
    }
    else if(childNum == 2)
    {
        FuncInfo *args = (FuncInfo*)(parent->other_info);
        TypeInfo *exp = (TypeInfo*)(parent->first_child->other_info);
        if(exp->sValid) 
        {
            Symbol *param = (Symbol*)malloc(sizeof(Symbol));
            param->type = exp->sType;
            param->dimension = exp->sDimension;
            param->next = NULL;
            param->p = parent->first_child;
            args->param_num++;
            Symbol *param_list = args->param_list;
            if(param_list == NULL)
                args->param_list = param;
            else
            {
                while(param_list->next != NULL)
                    param_list = param_list->next;
                param_list->next = param;
            }
        }
    }
    else if(childNum == 3)
    {
        child->other_info = parent->other_info;
    }
}

ID(58)
{
    if(childNum == 1)
    {
        TypeInfo* exp = (TypeInfo*)malloc(sizeof(TypeInfo));
        /* Demons Add */
        exp->iDimension = 0;
        /* Demons end */
        child->other_info = exp;
        child->IRIndex = nextVarIndex++; 
    }
}

SD(50)
{
    const char* func_name = parent->first_child->str + 4;
    FuncInfo *func_in_table = findFuncSymbol(func_name);
    FuncInfo *func_call = (FuncInfo*)(parent->first_child->other_info);
    if(func_in_table == NULL)
    {
        if(getSymbolFull(func_name) != NULL)
            printf("Error type 11 at Line %d: Value %s is not a function.\n",
                parent->loc_line, func_name);
        else
            printf("Error type 2 at Line %d: Function %s has not been defined.\n",
                parent->loc_line, func_name);
        TypeInfo* exp = (TypeInfo*)(parent->other_info);
        exp->sValid = 0;
        if(func_call->param_list != NULL)
            freeTempParamList(func_call->param_list);
        free(func_call);
        parent->first_child->other_info = NULL;
        return;
    }
    if(!checkFuncParamMatch(func_in_table, func_call))
        printf("Error type 9 at Line %d: Function \"%s\" call is not match its defination.\n", 
            parent->loc_line, func_name);
    else if(func_in_table->status == 0)
    {
        int old_size = func_in_table->use_line_size;
        func_in_table->use_line = expandFuncUseLine(func_in_table->use_line, old_size);
        func_in_table->use_line[old_size] = -(parent->loc_line);
        func_in_table->use_line_size = old_size + 1;
    }

    TypeInfo* exp = (TypeInfo*)(parent->other_info);
    exp->sType = func_in_table->return_type;
    exp->sDimension = 0;
    if(exp->sType == NULL)
        exp->sValid = 0;
    else
        exp->sValid = 1;
    exp->nextInfo = (void*)0;

// IR Generation
    Symbol *arg_list = func_call->param_list;
    AST_node *first_node = (AST_node*)(arg_list->p);
    char *func_id = parent->first_child->str + 4;
    if(strcmp(func_id, "write") == 0)
        gen_IR(Write, new_value(V, first_node->IRIndex));
    else
    {
        AST_node **node_array = (AST_node**)malloc(sizeof(AST_node*) * func_call->param_num);
        int i = 0;
        while(arg_list != NULL)
        {
            node_array[i] = (AST_node*)(arg_list->p);
            arg_list = arg_list->next;
            i++;
        }
        TypeInfo *cur_type;
        for(i = i - 1; i >= 0; i--) 
        {
            cur_type = (TypeInfo*)(node_array[i]->other_info);
            if(cur_type->sDimension > 0)
                gen_IR(Arg, new_value(Address, node_array[i]->IRIndex));
            else
                gen_IR(Arg, new_value(V, node_array[i]->IRIndex));
        }
    }
    int place_index = parent->IRIndex;
    AST_node *function = (AST_node*)(findFuncSymbol(func_id)->p);
    gen_IR(Call, new_value(V, place_index), new_value(F, function->IRIndex));
// end

    if(func_call->param_list != NULL)
        freeTempParamList(func_call->param_list);
    free(func_call);
    parent->first_child->other_info = NULL;
}

SD(51)
{
    const char* func_name = parent->first_child->str + 4;
    FuncInfo *func_in_table = findFuncSymbol(func_name);
    FuncInfo *func_call = (FuncInfo*)(parent->first_child->other_info);
    if(func_in_table == NULL)
    {
        if(getSymbolFull(func_name) != NULL)
            printf("Error type 11 at Line %d: Value %s is not a function.\n",
                parent->loc_line, func_name);
        else
            printf("Error type 2 at Line %d: Function %s has not been defined.\n",
                parent->loc_line, func_name);
        TypeInfo* exp = (TypeInfo*)(parent->other_info);
        exp->sValid = 0;
        if(func_call->param_list != NULL)
            freeTempParamList(func_call->param_list);
        free(func_call);
        parent->first_child->other_info = NULL;
        return;
    }
    if(!checkFuncParamMatch(func_in_table, func_call))
        printf("Error type 9 at Line %d: Function \"%s\" call is not match its defination.\n", 
            parent->loc_line, func_name);
    else if(func_in_table->status == 0)
    {
        int old_size = func_in_table->use_line_size;
        func_in_table->use_line = expandFuncUseLine(func_in_table->use_line, old_size);
        func_in_table->use_line[old_size] = -(parent->loc_line);
        func_in_table->use_line_size = old_size + 1;
    }

    TypeInfo* exp = (TypeInfo*)(parent->other_info);
    exp->sType = func_in_table->return_type;
    exp->sDimension = 0;
    if(exp->sType == NULL)
        exp->sValid = 0;
    else
        exp->sValid = 1;
    exp->nextInfo = (void*)0;

    if(func_call->param_list != NULL)
        freeTempParamList(func_call->param_list);
    free(func_call);
    parent->first_child->other_info = NULL;
}


SD(57)
{
    parent->other_info = NULL;
}

SD(58)
{
    FuncInfo *args = (FuncInfo*)(parent->other_info);
    TypeInfo *exp = (TypeInfo*)(parent->first_child->other_info);
    if(exp->sValid) 
    {
        Symbol *param = (Symbol*)malloc(sizeof(Symbol));
        param->type = exp->sType;
        param->dimension = exp->sDimension;
        param->next = NULL;
        param->p = parent->first_child;
        args->param_num++;
        Symbol *param_list = args->param_list;
        if(param_list == NULL)
            args->param_list = param;
        else
        {
            while(param_list->next != NULL)
                param_list = param_list->next;
            param_list->next = param;
        }
    }
    parent->other_info = NULL;
}


/**************************************************************/
/**************************************************************/
/* 处理return语句类型匹配 */

ID(23)
{
    if(childNum == 3)
    {
        TypeInfo *stmtList = (TypeInfo*)malloc(sizeof(TypeInfo));
        TypeInfo *compSt = (TypeInfo*)(parent->other_info);
        stmtList->sType = compSt->sType;
        stmtList->sValid = compSt->sValid;
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
        stmt->sValid = stmtList->sValid;
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
        compSt->sValid = stmt->sValid;
        child->other_info = compSt;
    }
}

ID(28)
{
    if(childNum == 2)
    {
        TypeInfo *exp = (TypeInfo*)malloc(sizeof(TypeInfo));
        exp->iDimension = 0;
        child->other_info = exp;
    }
}

ID(29)
{
    if(childNum == 0)
    {
        parent->labelIndex = nextLabelIndex;
        nextLabelIndex += 3;
    }

    if(childNum == 3)
    {
        TypeInfo *exp = (TypeInfo*)malloc(sizeof(TypeInfo));
        child->other_info = exp;
    }
    else if(childNum == 5)
    {
        /* IR tag */
        D_child_3;
        gen("IF v%d == 0 GOTO l%d\n", child_3->IRIndex, parent->labelIndex + 2);

        TypeInfo *stmt_ = (TypeInfo*)malloc(sizeof(TypeInfo));
        TypeInfo *stmt = (TypeInfo*)(parent->other_info);
        stmt_->sType = stmt->sType;
        stmt_->sValid = stmt->sValid;
        child->other_info = stmt_;
    }
}

ID(31)
{
    if(childNum == 0)
    {
        parent->labelIndex = nextLabelIndex;
        nextLabelIndex += 3;
    }
    if (childNum == 3)
    {
        gen("LABEL l%d :\n", parent->labelIndex);
    }
    if (childNum == 5)
    {
        D_child_3;
        gen("IF v%d == 0 GOTO l%d\n", child_3->IRIndex, parent->labelIndex + 2);
    }

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
        stmt_->sValid = stmt->sValid;
        child->other_info = stmt_;
    }
}

SD(31)
{
    gen("GOTO l%d\n", parent->labelIndex);
    gen("LABEL l%d :\n", parent->labelIndex + 2);
}

SD(29)
{
    D_child_3;
    gen("LABEL l%d :\n", parent->labelIndex + 2);
}

ID(30)
{
    if(childNum == 0)
    {
        parent->labelIndex = nextLabelIndex;
        nextLabelIndex += 3;
    }
    if(childNum == 5)
    {
        D_child_3;
        gen("IF v%d == 0 GOTO l%d\n", child_3->IRIndex, parent->labelIndex + 1);
    }
    if(childNum == 7)
    {
        gen("GOTO l%d\n", parent->labelIndex + 2);
        gen("LABEL l%d :\n", parent->labelIndex + 1);
    }

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
        stmt_->sValid = stmt->sValid;
        child->other_info = stmt_;
    }
}
SD(30)
{
    gen("LABEL l%d :\n", parent->labelIndex + 2);
}

SD(28)
{
    TypeInfo* stmt = (TypeInfo*)(parent->other_info);
    TypeInfo* exp = (TypeInfo*)(parent->first_child->next_brother->other_info);
    if(exp->sValid)
    {
        if(exp->sDimension != 0 
            || stmt->sType == NULL 
            || strcmp(exp->sType, stmt->sType) != 0)
            printf("Error type 8 at Line %d: Unmatch return value type.\n", parent->loc_line);
    }

}

/**************************************************************/
/**************************************************************/
/* 处理结构体定义和使用 */

ID(10)
{
    if(childNum == 1)
    {
        TypeInfo *structSpecifier = (TypeInfo*)malloc(sizeof(TypeInfo));
        structSpecifier->sValid = 1;
        child->other_info = structSpecifier;
    }
}


ID(11)
{
    if(childNum == 4)
    {
        AST_node *optTag = parent->first_child->next_brother;
        if(optTag->first_child == NULL)
        {
            stackPush(NULL, 1);
        }
        else
        {
            stackPush(optTag->first_child->str + 4, 0);
        }
    }
}

ID(53)
{
    if(childNum == 1)
    {
        D_parent_info;
        TypeInfo *exp_ = (TypeInfo*)malloc(sizeof(TypeInfo));
        /* exp_->iDimension = parent_info->iDimension; */
        exp_->iDimension = 0;
        child->other_info = exp_;
    }

}

SD(10)
{
    TypeInfo *specifier = (TypeInfo*)malloc(sizeof(TypeInfo));
    TypeInfo *structSpecifier = (TypeInfo*)(parent->first_child->other_info);
    if(!structSpecifier->sValid)
        specifier->sValid = 0;
    else {
        specifier->sValid = 1;
        specifier->sType = structSpecifier->sType;
    }
    parent->other_info = specifier;
}

SD(11)
{
    Symbol *struct_symbol = stackPop();
    TypeInfo *structSpecifier = (TypeInfo*)(parent->other_info);
    structSpecifier->sType = struct_symbol->name;
    structSpecifier->sValid = 1; 
}

SD(12)
{
    const char *struct_name = parent->first_child->next_brother->first_child->str + 4;
    Symbol *struct_symbol = getSymbolFull(struct_name);
    TypeInfo *structSpecifier = (TypeInfo*)(parent->other_info);
    if(struct_symbol == NULL) 
    {
        printf("Error type 17 at Line %d: Struct \"%s\" has not been defined.\n",
           parent->loc_line, struct_name);
        structSpecifier->sValid = 0;
    }
    else
    {
        structSpecifier->sValid = 1;
        structSpecifier->sType = struct_symbol->name; 
    }
}


SD(13)
{
    const char *struct_name = parent->first_child->str + 4;
    if(getSymbolFull(struct_name) != NULL)
        printf("Error type 16 at Line %d: Struct \"%s\" has been redefined.\n",
             parent->loc_line, struct_name);
}

SD(53)
{
    TypeInfo *exp_ = (TypeInfo*)(parent->first_child->other_info);
    TypeInfo *exp =  (TypeInfo*)(parent->other_info);
    if(exp_->sValid) 
    {
        if(strcmp(exp_->sType, "int") == 0 || strcmp(exp_->sType, "float") == 0 
            || exp_->sDimension != 0) 
        {
            printf("Error type 13 at Line %d: Exp is not a struct.\n", parent->loc_line);         
            exp->sValid = 0;            
          
            return;
        }
    }
    else {
        exp->sValid = 0;
        return;
    }
    const char* struct_name = exp_->sType;
    const char *region_id = parent->first_child->next_brother->next_brother->str + 4;
    Symbol *region_symbol = findRegionInStruct(struct_name, region_id);
    if(region_symbol == NULL) {
        printf("Error type 14 at Line %d: \"%s\" is not a region in struct \"%s\".\n",
             parent->loc_line, region_id, struct_name);
        exp->sValid = 0;
    }
    else {
        exp->sValid = 1;
        exp->sType = region_symbol->type;
        exp->sDimension = region_symbol->dimension - exp->iDimension;
        exp->nextInfo = (void*)1;
    }

}





void initTable_lxs()
{
    IS(6, 10, 11, 18, 20, 22, 23, 24, 27, 28, 29, 30, 31, 50, 51, 53, 57, 58, 59);
    SS(10, 11, 12, 13, 18, 19, 20, 21, 22, 28, 29, 30, 31, 50, 51, 53, 57, 58);
}
