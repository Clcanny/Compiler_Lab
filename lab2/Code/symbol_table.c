#include "symbol_table.h"
#include "SDTAction.h"

static int compSymbol(const void *p1, const void *p2)
{
    return strcmp(((Symbol *)p1)->name, ((Symbol *)p2)->name);
}

static int compFuncSymbol(const void *p1, const void *p2)
{
    return strcmp(((FuncInfo *)p1)->name, ((FuncInfo *)p2)->name);
}

static void *dupSymbol(void *p)
{
    void *dup_p;
    dup_p = calloc(1, sizeof(Symbol));
    memmove(dup_p, p, sizeof(Symbol));
    return dup_p;
}

static void *dupFuncSymbol(void *p)
{
    void *dup_p;
    void *dup_func_info;
    dup_func_info = calloc(1, sizeof(FuncInfo));
    memmove(dup_func_info, ((Symbol*)p)->next, sizeof(FuncInfo));
    dup_p = calloc(1, sizeof(Symbol));
    memmove(dup_p, p, sizeof(Symbol));
    ((Symbol*)dup_p)->next = dup_func_info;
    return dup_p;
}

static void deleteSymbol(void *p)
{
    free(p);
}

static void deleteFuncSymbol(void *p)
{
    FuncInfo* func = (FuncInfo*)p;
    free(func->use_line);
    free(func);
}

SymbolTable *newSymbolTable()
{
    jsw_rbtree_t *rbtree;
    rbtree = jsw_rbnew(compSymbol, dupSymbol, deleteSymbol);
    return rbtree;
}

void delSymbolTable(SymbolTable *st)
{
    jsw_rbdelete(st);
}

int insertSymbol(SymbolTable *st, const char *name, int kind,
 const char *type, int dimension, void* next_detail, void *p)
{
    int ret;
    
    Symbol *symbol = calloc(1, sizeof(Symbol));
    symbol->name = name;
    symbol->kind = kind;
    symbol->type = type;
    symbol->dimension = dimension;
    symbol->next = next_detail;
    symbol->p = p;
    
    ret = jsw_rbinsert(st, (void *)symbol);
    if (ret == 0)
    {
        printf("failed to insert the symbol with name %s and pointer %p\n", name, p);
        free(symbol);
        return -1;
    }
    free(symbol);
    return 0;
}

int eraseSymbol(SymbolTable *st, const char *name)
{
    int ret;
    Symbol *symbol = calloc(1, sizeof(Symbol));
    symbol->name = name;
    
    ret =jsw_rberase(st, (void *)symbol);
    if (ret == 0)
    {
        printf("failed to erase the symbol with name %s\n", name);
        free(symbol);
        return -1;
    }
    free(symbol);
    return 0;
}

void *findSymbol(SymbolTable *st, const char *name)
{
    Symbol *symbol, symbol_find;
    symbol_find.name = name;
    symbol = jsw_rbfind(st, &symbol_find);
    return symbol;
}

void printSymbolTable(SymbolTable *st)
{
    jsw_rbtrav_t *rbtrav;
    rbtrav = jsw_rbtnew();
    
    for (Symbol *symbol = jsw_rbtfirst(rbtrav, st);
            symbol != NULL;
            symbol = jsw_rbtnext(rbtrav))
    {
        printf("name: %-15s, kind: %d, type: %-15s, dimension: %d, pointer: %p\n",
             symbol->name, symbol->kind, symbol->type, symbol->dimension, symbol->p);
    }
    
    free(rbtrav);
}

int addSymbol(const char *name, AST_node *p)
{
    TypeInfo *info = (TypeInfo*)(p->other_info);
    return insertSymbol(globalSymbolTable, name, 0,
     info->sType, info->sDimension, NULL, (void *)p);
}

int delSymbol(const char *name)
{
    return eraseSymbol(globalSymbolTable, name);
}

AST_node *getSymbol(const char *name)
{
    void *p = findSymbol(globalSymbolTable, name);
    if (p == NULL)
    {
        return NULL;
    }
    else
    {
        return ((Symbol *)p)->p;
    }
}

void cleanUpSymbolTable()
{
    delSymbolTable(globalSymbolTable);
    delSymbolTable(globalFuncSymbolTable->table);
    free(globalFuncSymbolTable);
    free(globalStructStack);
}


FuncSymbolTable *newFuncSymbolTable()
{
    jsw_rbtree_t *rbtree;
    rbtree = jsw_rbnew(compFuncSymbol, dupFuncSymbol, deleteFuncSymbol);
    FuncSymbolTable *func_table = (FuncSymbolTable*)malloc(sizeof(FuncSymbolTable));
    func_table->table = rbtree;
    func_table->cur_def_func = NULL;
    func_table->func_in_table = NULL;
    return func_table;
}

void printFuncSymbolTable()
{
    jsw_rbtrav_t *rbtrav;
    rbtrav = jsw_rbtnew();
    FuncInfo *func_info;
    Symbol *param_list;
    
    for (FuncInfo *func_info = jsw_rbtfirst(rbtrav, globalFuncSymbolTable->table);
            func_info != NULL;
            func_info = jsw_rbtnext(rbtrav))
    {
        printf("name: %s, status: %d, return_type: %s, param_num: %d.\n", func_info->name,
         func_info->status, func_info->return_type, func_info->param_num);
        param_list = func_info->param_list;
        printf("Param List:\n");
        while(param_list != NULL)
        {
            printf("type: %s, dimension: %d\n", param_list->type, param_list->dimension);
            param_list = param_list->next;
        }
    }
    
    free(rbtrav);
}

void startDefineFunction(const char *name, int status, const char *return_type)
{
    FuncInfo *def_func = (FuncInfo*)malloc(sizeof(FuncInfo));
    def_func->name = name;
    def_func->status = status;
    def_func->return_type = return_type;
    def_func->use_line = NULL;
    def_func->use_line_size = 0;
    def_func->param_num = 0;
    def_func->param_list = NULL;
    globalFuncSymbolTable->cur_def_func = def_func;
    globalFuncSymbolTable->cur_add_param_tail = NULL;
    globalFuncSymbolTable->func_in_table = findFuncSymbol(name);
    globalFuncSymbolTable->is_defining = 1;
}

int funcDefineEnd(int line)
{
    FuncInfo *func_in_table = globalFuncSymbolTable->func_in_table,
        *cur_def_func = globalFuncSymbolTable->cur_def_func;
    int result;
    if(func_in_table == NULL) {
        result = 1;
        insertFuncIntoTable(cur_def_func);
    }
    else
    {
        if(func_in_table->status == 1 && cur_def_func->status == 1)
            result = 0;
        else 
        {
            if(checkFuncParamMatch() 
                && strcmp(func_in_table->return_type, cur_def_func->return_type) == 0)
            {
                if(cur_def_func->status == 0) 
                {
                    int old_size = func_in_table->use_line_size;
                    func_in_table->use_line = expandFuncUseLine(func_in_table->use_line, old_size);
                    func_in_table->use_line[old_size] = line;
                    func_in_table->use_line_size = old_size + 1;
                }
                result = 1;
            }
            else
                result = -1;
        }
    }
    free(globalFuncSymbolTable->cur_def_func->param_list);
    free(globalFuncSymbolTable->cur_def_func);
    globalFuncSymbolTable->cur_def_func = NULL;
    globalFuncSymbolTable->cur_add_param_tail = NULL;
    globalFuncSymbolTable->func_in_table = NULL;
    globalFuncSymbolTable->is_defining = 0;
    return result;
}

void addParamToTail(Symbol *param)
{
    if(globalFuncSymbolTable->cur_add_param_tail == NULL)
        globalFuncSymbolTable->cur_add_param_tail = param;
    else {
        globalFuncSymbolTable->cur_add_param_tail->next = param;
        globalFuncSymbolTable->cur_add_param_tail = param;
    }    
}

int addTempFuncParam(const char *param_name,
    const char *param_type, int param_dimension)
{
    // create a new param symbol
    Symbol *param = (Symbol*)malloc(sizeof(Symbol));
    param->name = param_name;
    param->kind = 2;
    param->type = param_type;
    param->dimension = param_dimension;
    param->next = NULL;
    globalFuncSymbolTable->cur_def_func->param_num += 1;

    Symbol *param_in_table = getSymbolFull(param_name);
    // 函数定义或声明第一次出现
    if(globalFuncSymbolTable->func_in_table == NULL) {
        addParamToTail(param);    
        if(param_in_table == NULL)
            return 1;
        else
            return 0;
    }

    // 函数定义或声明之前出现过
    // 参数和变量、结构体域名或者结构类型名重复
    if(param_in_table != NULL && param_in_table->kind != 2) {
        addParamToTail(param);
        return 0;
    }

    FuncInfo *function = globalFuncSymbolTable->cur_def_func;
    int param_list_index = 0;
    Symbol *param_new = function->param_list,
        *param_exist = globalFuncSymbolTable->func_in_table->param_list;
    while(param_new != NULL) {
        // 参数名和本函数其他参数重复
        if(strcmp(param_name, param_new->name) == 0) {
            if(globalFuncSymbolTable->cur_add_param_tail == NULL)
                globalFuncSymbolTable->cur_add_param_tail = param;
            else {
                globalFuncSymbolTable->cur_add_param_tail->next = param;
                globalFuncSymbolTable->cur_add_param_tail = param;
            }
            return 0;
        }
        param_list_index++;
        param_new = param_new->next;
    }
    while(param_exist != NULL) {
        param_list_index--;
        if(strcmp(param_name, param_exist->name) == 0)
            break;
        param_exist = param_exist->next;
    }

    // 参数和之前该函数的定义名字完全一致 
    if(param_list_index == 0 && param_exist != NULL) {
        addParamToTail(param);
        if(strcmp(param_type, param_exist->type) == 0
            && param_dimension == param_exist->dimension)
            return 1;
        else
            return 0;
    }
    else if(param_list_index != 0 && param_exist != NULL) {
        addParamToTail(param);
        return 0;
    }
    // 参数名和之前函数的定义名字完全不一致
    else if(param_list_index != 0 && param_exist == NULL) {
        addParamToTail(param);
        insertSymbol(globalSymbolTable, param_name, 2, param_type,
           param_dimension, NULL, NULL);
        return 1;
    }
    else
        return 0;
}

int *expandFuncUseLine(int *old_line, int old_size)
{
    int *new_line;
    if(old_line == NULL)
    {
        new_line = (int*)malloc(sizeof(int));
    }
    else
    {
        new_line = (int*)malloc(sizeof(int) * (old_size + 1));
        memmove(new_line, old_line, sizeof(int) * old_size);
        free(old_line);
    }
    return new_line;
}

// 将语法树节点中存的函数定义信息存入符号表
// 若返回1表示成功
// 返回0表示失败,函数重复定义; 
// 返回-1表示失败，函数多次声明相互冲突、声明和定义相互冲突
// int addNewFunc(const char *name, FuncInfo *function, int line)
// {
//     void *symbol = findSymbol(globalFuncSymbolTable->table, name);
//     // 符号表中不存在该函数名
//     if(symbol == NULL)
//     {
//         if(function->status == 0)
//         {
//             function->use_line = (int*)malloc(sizeof(int));
//             function->use_line[0] = line;
//             function->use_line_size = 1;
//         }
//         else
//         {
//             function->use_line = NULL;
//             function->use_line_size = 0;
//         }
//         Symbol *func_symbol = (Symbol*)malloc(sizeof(Symbol));
//         func_symbol->name = name;
//         func_symbol->kind = 2;
//         func_symbol->type = function->return_type;
//         func_symbol->next = function;
//         insertFuncIntoTable(func_symbol);
//         free(func_symbol);
//         return 1;
//     }
//     else 
//     {
//         Symbol *func_symbol = (Symbol*)symbol;
//         FuncInfo *func_in_table = (FuncInfo*)func_symbol->next;
//         if(func_symbol->kind != 2 
//             || (func_in_table->status == 1 && function->status == 1))
//             return 0;
//         if(strcmp(func_in_table->return_type, function->return_type) != 0)
//             return -1;
//         int check_result = checkFuncParamMatch(func_in_table, function);
//         // 修改函数定义声明状态
//         if(check_result == 1){
//             if(function->status == 1 && func_in_table->status == 0)
//                 func_in_table->status = 1;
//             else if(function->status == 0)
//             {
//                 int old_size = func_in_table->use_line_size;
//                 func_in_table->use_line = expandFuncUseLine(func_in_table->use_line, old_size);
//                 func_in_table->use_line[old_size] = line;
//                 func_in_table->use_line_size = old_size + 1;
//             }
//         }
//         return (check_result == 0 ? -1 : 1);
//     }
// }

int insertFuncIntoTable(FuncInfo *function)
{
    int result;
    // 将function的param插入符号表
    Symbol *param = function->param_list;
    Symbol *first_param = param, *cur_param = param;
    if(param != NULL)
    {
        insertSymbol(globalSymbolTable, param->name, param->kind, 
            param->type, param->dimension, param->next, param->p);
        cur_param = (Symbol*)findSymbol(globalSymbolTable, param->name);
        function->param_list = cur_param;
        while(param->next != NULL)
        {
            param = param->next;
            insertSymbol(globalSymbolTable, param->name, param->kind, 
                param->type, param->dimension, param->next, param->p);
            cur_param->next = (Symbol*)findSymbol(globalSymbolTable, param->name);
            cur_param = cur_param->next;
        }
    }

    // 将函数信息插入符号表
    result = jsw_rbinsert(globalFuncSymbolTable->table, (void *)function);
    // 使function中的参数指针仍然指向临时参数列表
    function->param_list = first_param;
    if (result == 0)
    {
        printf("failed to insert the function symbol with name %s\n",
            function->name);
        return -1;
    }        
    return 0;
}

void freeTempParamList(Symbol *param_list)
{
    Symbol *cur_param = param_list;
    while(param_list != NULL)
    {
        cur_param = param_list;
        param_list = param_list->next;
        free(cur_param);
    } 
}

int checkFuncParamMatch()
{
    FuncInfo *func_exist = globalFuncSymbolTable->func_in_table, 
        *func_uncheck = globalFuncSymbolTable->cur_def_func;
    if(func_exist->param_num != func_uncheck->param_num)
        return 0;
    Symbol *exi_param = func_exist->param_list;
    Symbol *unc_param = func_uncheck->param_list;
    while(exi_param != NULL)
    {
        if(strcmp(exi_param->type, unc_param->type) != 0
            || exi_param->dimension != unc_param->dimension)
            return 0;
        exi_param = exi_param->next;
        unc_param = unc_param->next;
    }
    return 1;
}

FuncInfo *findFuncSymbol(const char *func_name)
{
    FuncInfo *symbol, symbol_find;
    symbol_find.name = func_name;
    symbol = jsw_rbfind(globalFuncSymbolTable->table, &symbol_find);
    return symbol;
}

void findUndefinedFunction()
{
    jsw_rbtrav_t *rbtrav;
    rbtrav = jsw_rbtnew();
    for (FuncInfo *func_info = jsw_rbtfirst(rbtrav, globalFuncSymbolTable->table);
            func_info != NULL;
            func_info = jsw_rbtnext(rbtrav))
    {
        if(func_info->status == 0)
        {
            int n = func_info->use_line_size;
            int *use_line = func_info->use_line;
            while(n > 0)
            {
                n--;
                if(use_line[n] > 0)
                    printf("Error type 18 at Line %d: Function \"%s\" has been declared but not been defined.\n",
                     use_line[n], func_info->name);
                else
                    printf("Error type 2 at Line %d: Function \"%s\" has not been defined.\n",
                        -(use_line[n]), func_info->name);
            }

        }
    }
    
    free(rbtrav);
}


StructStack *newStructStack()
{
    StructStack *structStack = (StructStack*)malloc(sizeof(StructStack));
    structStack->anonymous_struct_n = 0;
    structStack->stack_top = NULL;
    structStack->isEmpty = stackIsEmpty;
    structStack->addRegion = stackAddRegion;
    structStack->push = stackPush;
    structStack->pop = stackPop;
    return structStack;
}

int stackIsEmpty()
{
    return globalStructStack->stack_top == NULL;
}

int stackAddRegion(const char *region_name, void *type_info)
{
    if(globalStructStack->isEmpty())
        return -1;
    TypeInfo *region_info = (TypeInfo*)type_info;
    if(findRegionInStruct(globalStructStack->stack_top->struct_symbol->name, region_name) != NULL)
        return 0;
    insertSymbol(globalSymbolTable, region_name, 0, region_info->sType,
        region_info->sDimension, NULL, type_info);
    Symbol *new_region = (Symbol*)findSymbol(globalSymbolTable, region_name);

    globalStructStack->stack_top->last_region->next = new_region;
    globalStructStack->stack_top->last_region = new_region;
    return 1;
}

void stackPush(const char *struct_name, int is_anonymous)
{
    if(is_anonymous){
        char *new_name = (char*)malloc(sizeof(char) * MAX_ANONYMOUS_STRUCT_LENGTH);
        sprintf(new_name, "struct-%d", globalStructStack->anonymous_struct_n);
        globalStructStack->anonymous_struct_n++;
        struct_name = new_name;
    }
    StackElement *element = (StackElement*)malloc(sizeof(StackElement));
    insertSymbol(globalSymbolTable, struct_name, 1, NULL, 0, NULL, NULL);
    element->struct_symbol = (Symbol*)findSymbol(globalSymbolTable, struct_name);
    element->last_region = element->struct_symbol;
    element->down = globalStructStack->stack_top;
    globalStructStack->stack_top = element;
}

Symbol *stackPop()
{
    if(globalStructStack->isEmpty())
        return NULL;
    StackElement *top_element  = globalStructStack->stack_top;
    Symbol *top_symbol = top_element->struct_symbol;
    globalStructStack->stack_top = top_element->down;
    free(top_element);
    return top_symbol;
}

Symbol *findRegionInStruct(const char *struct_name, const char *region_name)
{
    Symbol *struct_region = (Symbol*)findSymbol(globalSymbolTable, struct_name);
    while(struct_region->next != NULL)
    {
        struct_region = (Symbol*)(struct_region->next);
        if(strcmp(struct_region->name, region_name) == 0)
            return struct_region;
    }
    return NULL;
}

Symbol *getSymbolFull(const char *name)
{
    void *p = findSymbol(globalSymbolTable, name);
    if (p == NULL)
    {
        return NULL;
    }
    else
    {
        return (Symbol *)p;
    }
}