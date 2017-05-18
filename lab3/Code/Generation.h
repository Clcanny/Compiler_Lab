#ifndef Generation_H
#define Generation_H

#define gen printf

#include "list.h"

enum ValueKind {
    V = 0,     // 普通变量, v1
    T = 1,         // 临时变量, t1
    L = 2,        // label, l1
    F = 3,          // Function, f1
    Const = 4,     // 立即数, #1
    Address = 5,      // 地址, &x
    Content = 6,      // 地址中的内容, *x 
}; 

typedef struct {
    enum ValueKind kind;
    union {
        int no;     // 变量标号
        int value;  // 立即数的值
    } u;
    char *str;
} Value;

Value* new_value(int kind, int value);
void free_value(Value*);

enum IRKind{
    Label = 0,      // "LABEL target :"
    Fun = 1,        // "FUNCTION target :"
    Calculate = 2,  // "target := arg1 u.op arg2"
    Assign = 3,     // "target := arg1"    "target := &arg1" 
                    // "target := *arg1"   "*target := arg1"
    Goto = 4,       // "GOTO target"
    GotoRel = 5,    // "IF arg1 u.relop arg2 GOTO target"
    Return = 6,     // "RETURN target"
    Dec = 7,        // "DEC target arg1"
    Arg = 8,        // "ARG target"
    Call = 9,       // "target := CALL arg1"
    Param = 10,     // "PARAM target"
    Read = 11,      // "READ target"
    Write = 12,     // "WRITE target"
};

typedef struct {
    enum IRKind kind;

    Value *target;
    union {
        char* op;
        char* relop;  
    }u;
    Value *arg1;
    Value *arg2;
}IR;

list_t *IR_list;

IR* gen_IR(int kind, Value *target, ...);
void free_IR(void *val);
list_t *new_IR_list();
void del_IR_list();
void traverse_IR_list(void (*action)(IR*));
void print_IR(IR*);

#endif
