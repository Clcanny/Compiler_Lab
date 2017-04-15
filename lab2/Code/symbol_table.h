#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "jsw_rbtree.h"
#include "syntax_tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    const char *name;
    
    // kind：0表示name为变量名、结构体域名或者形参名，1表示name为结构体名，2表示name为函数名
    int kind; 

    // type也可以表示函数的返回值类型
    const char *type;

    // 只在kind=0或者1时有效，表示维度
    int dimension;

    union
    {
        // next用于连接同属于一个struct的域，或者连接同一个函数的形参列表
        void *next;

        // 当kind=1时，detail指向StructInfo; 当kind=2时，detail指向FuncInfo
        void *detail; 
    } u;

    void *p;
} Symbol;

typedef struct
{
    int status; // 0表示为函数声明，1表示为函数定义
    const char *return_type;
    int *use_line; // 函数声明或者使用所在行数, 为正代表声明，为负代表使用
    int use_line_size; // use_line数组的大小
    int param_num;
    Symbol *param_list;
} FuncInfo;

typedef struct 
{
    Symbol *field_list;
} StructInfo;


typedef struct jsw_rbtree SymbolTable;

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


SymbolTable *globalFuncSymbolTable;

void printFuncSymbolTable();
SymbolTable *newFuncSymbolTable();
void addTempFuncParam(FuncInfo *function, const char *param_name,
 const char*param_type, int param_dimension);
int *expandFuncUseLine(int *old_line, int old_size);
// 清理临时参数列表
void freeTempParamList(Symbol *param_list);
// 将语法树节点中存的函数定义信息存入符号表
// 若返回1表示成功
// 返回0表示失败,函数重复定义; 
// 返回-1表示失败，函数多次声明相互冲突、声明和定义相互冲突
int addNewFunc(const char *name, FuncInfo *function, int line);
int insertFuncIntoTable(Symbol *function);
int checkFuncParamMatch(FuncInfo *func_exist, FuncInfo *func_uncheck);
// 在函数符号表中查找相应函数信息
Symbol *getFuncSymbol(const char *func_name);
// 在所有SDT执行完后, 查看函数符号表确认是否有函数未定义
void findUndefinedFunction();

#endif
