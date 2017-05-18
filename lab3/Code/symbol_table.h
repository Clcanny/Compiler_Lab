#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "jsw_rbtree.h"
#include "syntax_tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct jsw_rbtree SymbolTable;

typedef struct
{
    const char *name;
    
    // kind：0表示name为普通变量名、结构体域名，1表示name为结构类型名，2表示为形参名
    int kind; 

    // kind=0时有效，表类型名
    const char *type;

    // 只在kind=0时有效，表示维度
    int dimension;
    void *next;

    void *p;
} Symbol;

typedef struct
{
    const char *name; //函数名
    int status; // 0表示为函数声明，1表示为函数定义
    const char *return_type;
    int *use_line; // 函数声明或者使用所在行数, 为正代表声明，为负代表使用
    int use_line_size; // use_line数组的大小
    int param_num;
    Symbol *param_list;

    void *p;
} FuncInfo;

typedef struct
{
    SymbolTable *table;
    FuncInfo *cur_def_func; // 当前正处于定义状态的函数
    FuncInfo *func_in_table;
    int is_defining;

} FuncSymbolTable;

typedef struct stack_element
{
    Symbol *struct_symbol;
    Symbol *last_region;
    struct stack_element *down; // 指向更靠近栈底的一个元素
} StackElement;

#define MAX_ANONYMOUS_STRUCT_LENGTH 10

typedef struct
{
    int anonymous_struct_n; // 无名结构体个数，无名结构体以struct-1形式存在符号表中
    StackElement *stack_top;

    int (*isEmpty)(); //0为false, 1为true
    // 在结构体中时，定义一个新变量应调用这个函数，无需插入符号表
    int (*addRegion)(const char *region_name, void *type_info); 

    void (*push)(const char *struct_name, int is_anonymous);
    Symbol* ((*pop)());

} StructStack;

StructStack *globalStructStack;
StructStack *newStructStack();
//0为false, 1为true
int stackIsEmpty();
// 在结构体中时，定义一个新变量时应调用这个函数，无需插入符号表
int stackAddRegion(const char *region_name, void *type_info);
void stackPush(const char *struct_name, int is_anonymous);
Symbol *stackPop();
Symbol *findRegionInStruct(const char *struct_name, const char *region_name);
Symbol *getSymbolFull(const char *name);

/***************************************/
// Normal Symbol


SymbolTable *newSymbolTable();
void delSymbolTable(SymbolTable *st);

int insertSymbol(SymbolTable *st, const char *name, int kind,
 const char *type, int dimension, void* next_detail, void *p);
int eraseSymbol(SymbolTable *st, const char *name);
void *findSymbol(SymbolTable *st, const char *name);
void printSymbolTable(SymbolTable *st);

SymbolTable *globalSymbolTable;

int addSymbol(const char *name, AST_node *p);
int delSymbol(const char *name);
AST_node *getSymbol(const char *name);
void cleanUpSymbolTable();

/*******************************************/
// funcSymbol
FuncSymbolTable *globalFuncSymbolTable;

int isDefineFunction();
void printFuncSymbolTable();
FuncSymbolTable *newFuncSymbolTable();
void startDefineFunction(const char *name, int status, const char *return_type, void *p);
// 返回1表示成功，返回0表示函数重定义，返回-1表示函数声明与定义间不匹配
int funcDefineEnd(int line);
// 返回1代表成功，返回0代表失败
int addTempFuncParam(const char *param_name,
    const char *param_type, int param_dimension);
// 清理临时参数列表
void freeTempParamList(Symbol *param_list);

int *expandFuncUseLine(int *old_line, int old_size);

int insertFuncIntoTable(FuncInfo *function);
int checkFuncParamMatch(FuncInfo *func_exist, FuncInfo *func_uncheck);
FuncInfo *findFuncSymbol(const char *func_name);
// 在所有SDT执行完后, 查看函数符号表确认是否有函数未定义
void findUndefinedFunction();

#endif
