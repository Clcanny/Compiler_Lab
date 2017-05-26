#ifndef Generation_H
#define Generation_H

#define gen printf

#include <stdio.h>
#include "list.h"

FILE *f_output;

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
void new_IR_list();
void del_IR_list();
void traverse_list(list_t *list, void (*action)(list_node_t*));
void print_IR(list_node_t *ir_node);
void write_IR(list_node_t *ir_node);

void generate_jump_target(int label_count, int func_count);
void peep_hole();

struct basis_block;
typedef struct cfg_edge {
    struct basis_block *target;
    struct cfg_edge *next;
} CFG_edge;

typedef struct basis_block {
    int index;
    list_node_t *first_ir, *last_ir;
    int ir_count;
    CFG_edge *next; // 后继节点
    CFG_edge *prev; // 前驱节点
    int next_count, prev_count;
} BasisBlock;
list_t *block_list;

typedef struct {
    list_node_t *target_ir;
    BasisBlock *target_block;
    int goto_count;
    int goto_rel_count;
} JumpTarget;
JumpTarget *label_jump;

typedef struct {
    list_node_t *target_ir;
    BasisBlock *target_block;
    int call_count;
} CalTarget;
CalTarget *func_jump;

typedef struct control_flow_graph {
    BasisBlock *entry;
    BasisBlock *exit;
} CFG;
CFG *cfg;

void free_basis_block(void *val);
BasisBlock *new_basis_block();
void new_block_list();
void del_block_list();
void generate_CFG();

void generate_example_ir();

#endif
