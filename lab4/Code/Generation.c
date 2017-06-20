#include "list.h"
#include "Generation.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

Value* new_value(int kind, int value) 
{
    Value *new_value = (Value*)malloc(sizeof(Value));
    new_value->kind = kind;
    if(kind == Const)
        new_value->u.value = value;
    else
        new_value->u.no = value;
    char *value_str = (char*)malloc(sizeof(char) * 10);
    switch(kind)
    {
        case 0:
            sprintf(value_str, "v%d", value);
            break;
        // case 1:
        //     sprintf(value_str, "t%d", value);
        //     break;
        case 2:
            sprintf(value_str, "l%d", value);
            break;
        case 3:
        {
            if(value == 0)
                sprintf(value_str, "main");
            else
                sprintf(value_str, "f%d", value);
            break;
        }
        case 4:
            sprintf(value_str, "#%d", value);
            break;
        case 5:
            sprintf(value_str, "&v%d", value);
            break;
        case 6:
            sprintf(value_str, "*v%d", value);
            break;
    }
    new_value->str = value_str;
    return new_value;
}

void free_value(Value *value)
{
    if(value == NULL)
        return;
    free(value->str);
    free(value);
}

IR* gen_IR(int kind, Value *target, ...)
{
    if(kind < 0 || kind >12)
        return NULL;
    IR *ir = (IR*)malloc(sizeof(IR));
    ir->kind = kind;
    ir->target = target;
    ir->arg1 = NULL;
    ir->arg2 = NULL;
    ir->u.op = NULL;
    va_list ap;
    va_start(ap, target);
    switch(kind)
    {
        case 0:case 1:case 4:case 6:case 8:case 10:case 11:case 12:
            break;
        case 3:case 7: case 9:
            ir->arg1 = va_arg(ap, Value*);
            break;
        case 2:
            ir->arg1 = va_arg(ap, Value*);
            ir->arg2 = va_arg(ap, Value*);
            ir->u.op = va_arg(ap, char*);
            break;
        case 5:
            ir->arg1 = va_arg(ap, Value*);
            ir->arg2 = va_arg(ap, Value*);
            ir->u.relop = va_arg(ap, char*);
            break;
    }
    va_end(ap);
    list_rpush(IR_list, list_node_new(ir));
    return ir;
}

void free_IR(void *val)
{
    if(val == NULL)
        return;
    IR *ir = (IR*)val;
    free_value(ir->target);
    free_value(ir->arg1);
    free_value(ir->arg2);
    free(ir);
}

void new_IR_list()
{
    IR_list = list_new(free_IR);
}

void del_IR_list()
{
    list_destroy(IR_list);
}

void traverse_list(list_t *list, void (*action)(list_node_t*))
{
    list_node_t *node;
    list_iterator_t *it = list_iterator_new(list, LIST_HEAD);
    while ((node = list_iterator_next(it))) 
    {
        action(node);
    }
    list_iterator_destroy(it);
}

void print_IR(list_node_t *ir_node)
{
    IR *ir = (IR*)(ir_node->val);
    switch(ir->kind)
    {
        case 0:
            gen("LABEL %s :\n", ir->target->str);
            break;
        case 1:
            gen("FUNCTION %s :\n", ir->target->str);
            break;
        case 2:
            gen("%s := %s %s %s\n", ir->target->str, 
                ir->arg1->str, ir->u.op, ir->arg2->str);
            break;
        case 3:
            gen("%s := %s\n", ir->target->str, ir->arg1->str);
            break;
        case 4:
            gen("GOTO %s\n", ir->target->str);
            break;
        case 5:
            gen("IF %s %s %s GOTO %s\n", ir->arg1->str,
                ir->u.relop, ir->arg2->str, ir->target->str);
            break;
        case 6:
            gen("RETURN %s\n", ir->target->str);
            break;
        case 7:
            gen("DEC %s %d\n", ir->target->str, ir->arg1->u.value);
            break;
        case 8:
            gen("ARG %s\n", ir->target->str);
            break;
        case 9:
            gen("%s := CALL %s\n", ir->target->str, ir->arg1->str);
            break;
        case 10:
            gen("PARAM %s\n", ir->target->str);
            break;
        case 11:
            gen("READ %s\n", ir->target->str);
            break;
        case 12:
            gen("WRITE %s\n", ir->target->str);
            break;
    }
}

// void write_IR(list_node_t *ir_node)
// {
//     IR *ir = (IR*)(ir_node->val);
//     switch(ir->kind)
//     {
//         case 0:
//             fprintf(f_output, "LABEL %s :\n", ir->target->str);
//             break;
//         case 1:
//             fprintf(f_output, "FUNCTION %s :\n", ir->target->str);
//             break;
//         case 2:
//             fprintf(f_output, "%s := %s %s %s\n", ir->target->str, 
//                 ir->arg1->str, ir->u.op, ir->arg2->str);
//             break;
//         case 3:
//             fprintf(f_output, "%s := %s\n", ir->target->str, ir->arg1->str);
//             break;
//         case 4:
//             fprintf(f_output, "GOTO %s\n", ir->target->str);
//             break;
//         case 5:
//             fprintf(f_output, "IF %s %s %s GOTO %s\n", ir->arg1->str,
//                 ir->u.relop, ir->arg2->str, ir->target->str);
//             break;
//         case 6:
//             fprintf(f_output, "RETURN %s\n", ir->target->str);
//             break;
//         case 7:
//             fprintf(f_output, "DEC %s %d\n", ir->target->str, ir->arg1->u.value);
//             break;
//         case 8:
//             fprintf(f_output, "ARG %s\n", ir->target->str);
//             break;
//         case 9:
//             fprintf(f_output, "%s := CALL %s\n", ir->target->str, ir->arg1->str);
//             break;
//         case 10:
//             fprintf(f_output, "PARAM %s\n", ir->target->str);
//             break;
//         case 11:
//             fprintf(f_output, "READ %s\n", ir->target->str);
//             break;
//         case 12:
//             fprintf(f_output, "WRITE %s\n", ir->target->str);
//             break;
//     }
// }

void generate_jump_target(int label_count, int func_count)
{
    if(label_count > 0) {
        label_jump = (JumpTarget*)malloc(sizeof(JumpTarget) * label_count);
        for(int i = 0; i < label_count; i++) {
            label_jump[i].target_ir = NULL;
            label_jump[i].target_block = NULL;
            label_jump[i].goto_count = 0;
            label_jump[i].goto_rel_count = 0;
        }
    }
    else
        label_jump = NULL;
    if(func_count > 0) {
        func_jump = (CalTarget*)malloc(sizeof(CalTarget) * func_count);
        for(int i = 0; i < func_count; i++) {
            func_jump[i].target_ir = NULL;
            func_jump[i].target_block = NULL;
            func_jump[i].call_count = 0;
        }
    }
    else
        func_jump = NULL;
    int label_index = 0, func_index = 0;
    list_node_t *node;
    IR *ir;
    list_iterator_t *it = list_iterator_new(IR_list, LIST_HEAD);
    while ((node = list_iterator_next(it))) 
    {
        ir = (IR*)(node->val);
        if(ir->kind == Fun && func_jump != NULL) {
            // 排除main函数
            if(ir->target->u.no == 0)
                continue;
            func_jump[ir->target->u.no - 1].target_ir = node;
            assert(func_index < func_count);
            func_index++;
        }
        else if(ir->kind == Label && label_jump != NULL) {
            label_jump[ir->target->u.no - 1].target_ir = node;
            assert(label_index < label_count);
            label_index++;
        }
        else if(ir->kind == Call && func_jump != NULL) {
            func_jump[ir->arg1->u.no - 1].call_count++;
        }
        else if(ir->kind == Goto && label_jump != NULL) {
            label_jump[ir->target->u.no - 1].goto_count++;
        }
        else if(ir->kind == GotoRel && label_jump != NULL) {
            label_jump[ir->target->u.no - 1].goto_rel_count++;
        }
    }
    list_iterator_destroy(it);
}

// 控制流优化
void peep_hole_control(list_node_t *cur_node)
{
    IR *ir = (IR*)(cur_node->val);
    list_node_t *n_n_node;
    IR *n_n_ir;
    if(ir->kind == Goto) 
    {
        // remove ir whose kind is not Label
        if(cur_node->next != NULL) 
        {
            list_node_t *n_node, *rm_node;
            n_node = rm_node = cur_node->next;
            IR *n_ir = (IR*)(n_node->val);
            while(n_ir->kind != Label) 
            {
                n_node = n_node->next;
                list_remove(IR_list, rm_node);
                if(n_node == NULL)
                    break;
                else
                    n_ir = (IR*)(n_node->val);
            }
        }
    }
    if(ir->kind == Goto || ir->kind == GotoRel)
    {
        n_n_ir = NULL;
        int jump_target;
        do 
        {        
            if(n_n_ir != NULL) {
                jump_target = n_n_ir->target->u.no;
                if(ir->kind == Goto) {
                    label_jump[jump_target].goto_count++;
                    label_jump[ir->target->u.no].goto_count--;
                }
                else {
                    label_jump[jump_target].goto_rel_count++;
                    label_jump[ir->target->u.no].goto_rel_count--;
                }               
                ir->target->u.no = n_n_ir->target->u.no;
                ir->target->str = n_n_ir->target->str;
            }
            n_n_node = label_jump[ir->target->u.no - 1].target_ir->next;
            if(n_n_node == NULL)
                break;
            n_n_ir = (IR*)(n_n_node->val);
        } while(n_n_ir->kind == Goto);
    }
}

char *opposite_relop[6] = {"==", ">=", ">", "<=", "<", "!="};
// 消除不可达代码
void peep_hole_inaccess(list_node_t *cur_node)
{
    IR *ir = (IR*)(cur_node->val);
    if(ir->kind == GotoRel)
    {
        int l1 = ir->target->u.no, l2;
        list_node_t *n_node = cur_node->next;
        IR *n_ir;
        if(n_node == NULL || (n_ir = (IR*)(n_node->val))->kind != Goto)
            return;
        l2 = n_ir->target->u.no;
        if(n_node->next == NULL
            ||((IR*)(n_node->next->val))->target->u.no != l1
            || label_jump[l1 - 1].goto_rel_count > 1
            || label_jump[l1 - 1].goto_count > 0)
            return;
        int i;
        for(i = 0; i < 6; i++) {
            if(strcmp(opposite_relop[i], ir->u.relop) == 0) 
                break;
        }
        if(i == 6)
            return;
        else
            ir->u.relop = opposite_relop[5 - i];
        ir->target->u.no = l2;
        free(ir->target->str);
        char *target_str = (char*)malloc(sizeof(char)*10);
        sprintf(target_str, "l%d", l2);
        ir->target->str = target_str;
        list_remove(IR_list, n_node->next);
        list_remove(IR_list, n_node);
    }
}

void peep_hole_constant(list_node_t *cur_node)
{
    IR *ir = (IR*)(cur_node->val);
}

void peep_hole()
{
    // generate_example_ir();
    // traverse_list(IR_list, print_IR);
    printf(">>>>>>>>>>>>>>>>>>>>\n");
    traverse_list(IR_list, peep_hole_control);
    traverse_list(IR_list, peep_hole_inaccess);
    // traverse_list(IR_list, print_IR);
    // printf("<<<<<<<<<<<<<<<<<<<\n");
    // free(label_jump);
    // free(func_jump);
}

void free_basis_block(void *val)
{
    BasisBlock *block = (BasisBlock*)val;
    CFG_edge *cur_edge, *rm_edge;
    cur_edge = rm_edge = block->next;
    while(cur_edge != NULL) {
        rm_edge = cur_edge;
        cur_edge = cur_edge->next;
        free(rm_edge);
    }
    cur_edge = rm_edge = block->prev;
    while(cur_edge != NULL) {
        rm_edge = cur_edge;
        cur_edge = cur_edge->next;
        free(rm_edge);
    }
    free(block);
}

BasisBlock *new_basis_block()
{
    BasisBlock *block = (BasisBlock*)malloc(sizeof(BasisBlock));
    block->index = 0;
    block->first_ir = NULL;
    block->last_ir = NULL;
    block->ir_count = 0;
    block->next = NULL;
    block->prev = NULL;
    block->next_count = 0;
    block->prev_count = 0;
    return block;
}

void new_block_list()
{
    block_list = list_new(free_basis_block);
}

void del_block_list()
{
    list_destroy(block_list);
}

void print_Block(list_node_t *block_node)
{
    BasisBlock *block = (BasisBlock*)(block_node->val);
    printf("Block-%d: \n", block->index);
    printf("    first_ir: ");
    print_IR(block->first_ir);
    printf("    last_ir： ");
    print_IR(block->last_ir);
    printf("    count: %d\n", block->ir_count);
    CFG_edge *edge = block->next;
    printf("    next_edge--%d: ", block->next_count);
    while(edge != NULL) {
        printf("Block-%d  ", edge->target->index);
        edge = edge->next;
    }
    edge = block->prev;
    printf("\n    prev_edge--%d: ", block->prev_count);
    while(edge != NULL) {
        printf("Block-%d  ", edge->target->index);
        edge = edge->next;
    }
    printf("\n");
}

void add_prev_edge(BasisBlock *cur_block, BasisBlock *target_block)
{
    if(cur_block == NULL || target_block == NULL)
        return;
    CFG_edge *e = (CFG_edge*)malloc(sizeof(CFG_edge));
    e->target = target_block;
    e->next = NULL;
    CFG_edge *cur_edge = cur_block->prev;
    if(cur_edge == NULL)
        cur_block->prev = e;
    else {
        while(cur_edge->next != NULL)
            cur_edge = cur_edge->next;
        cur_edge->next = e;
    }
    cur_block->prev_count++;    
}

void add_next_edge(BasisBlock *cur_block, BasisBlock *target_block)
{
    if(cur_block == NULL || target_block == NULL)
        return;
    CFG_edge *e = (CFG_edge*)malloc(sizeof(CFG_edge));
    e->target = target_block;
    e->next = NULL;
    CFG_edge *cur_edge = cur_block->next;
    if(cur_edge == NULL)
        cur_block->next = e;
    else {
        while(cur_edge->next != NULL)
            cur_edge = cur_edge->next;
        cur_edge->next = e;
    }
    cur_block->next_count++;
}

void generate_CFG_edge(list_node_t *block_node)
{
    BasisBlock *cur_block = (BasisBlock*)(block_node->val);
    IR *last_ir = (IR*)(cur_block->last_ir->val);
    BasisBlock *target_block = NULL;
    if(last_ir->kind == Goto) 
    {
        target_block = label_jump[last_ir->target->u.no - 1].target_block;
    }
    else if(last_ir->kind == GotoRel)
    {
        target_block = label_jump[last_ir->target->u.no - 1].target_block;
        if(block_node->next != NULL) {
            BasisBlock *next_block = (BasisBlock*)(block_node->next->val);
            add_next_edge(cur_block, next_block);
            add_prev_edge(next_block, cur_block);
        }
    }
    else if(last_ir->kind == Call)
    {
        target_block = func_jump[last_ir->arg1->u.no - 1].target_block;
    }
    else
        return;
    add_next_edge(cur_block, target_block);
    add_prev_edge(target_block, cur_block);
}

void generate_CFG()
{
    // cfg = (CFG*)malloc(sizeof(CFG));
    // cfg->entry = new_basis_block();
    // cfg->exit = new_basis_block();
    new_block_list();

    // generate Block list
    BasisBlock *cur_block;
    int ir_count = 0;
    int block_count = 1;

    list_node_t *node;
    list_iterator_t *it = list_iterator_new(IR_list, LIST_HEAD);
    IR *ir;
    while ((node = list_iterator_next(it))) 
    {
        ir = (IR*)(node->val);
        ir_count++;
        if(node == IR_list->head) {
            cur_block = new_basis_block();
            cur_block->index = block_count++;
            cur_block->first_ir = node;
        }
        else if(ir->kind == Fun || ir->kind == Label) {
            cur_block->last_ir = node->prev;
            cur_block->ir_count = ir_count - 1;
            if(cur_block->ir_count > 0) {
                list_rpush(block_list, list_node_new(cur_block));
            }
            else {
                block_count--;
                free(cur_block);
                cur_block = NULL;
            }
            if(node->next != NULL) {
                cur_block = new_basis_block();
                cur_block->index = block_count++;
                cur_block->first_ir = node;
                if(ir->kind == Label) {
                    assert(label_jump[ir->target->u.no - 1].target_block == NULL);
                    label_jump[ir->target->u.no - 1].target_block = cur_block;
                }
                else {
                    if(ir->target->u.no > 0) {
                        assert(func_jump[ir->target->u.no - 1].target_block == NULL);
                        func_jump[ir->target->u.no - 1].target_block = cur_block;                        
                    }                
                }
            }
            ir_count = 1;
        }
        else if(ir->kind == Call || ir->kind == Goto 
            || ir->kind == GotoRel || ir->kind == Return) {
            cur_block->last_ir = node;
            cur_block->ir_count = ir_count;
            if(cur_block->ir_count > 0)
                list_rpush(block_list, list_node_new(cur_block));
            else {
                block_count--;
                free(cur_block);
                cur_block = NULL;
            }
            if(node->next != NULL) {
                cur_block = new_basis_block();
                cur_block->index = block_count++;
                cur_block->first_ir = node->next;
            }
            ir_count = 0;
        }
    }
    list_iterator_destroy(it);
    // cur_block->last_ir = IR_list->tail;
    // cur_block->ir_count = ir_count;
    // if(cur_block->ir_count > 0)
    //     list_rpush(block_list, list_node_new(cur_block));
    // else {
    //     block_count--;
    //     free(cur_block);
    // }
    traverse_list(block_list, generate_CFG_edge);
    traverse_list(block_list, print_Block);

}


void generate_example_ir()
{
    printf("*****************\n");
    del_IR_list();
    new_IR_list();
    
    gen_IR(Fun, new_value(F, 1));
    gen_IR(GotoRel, new_value(L, 1), new_value(V, 1), new_value(V, 2), "==");
    // gen_IR(Label, new_value(L, 1));
    gen_IR(Goto, new_value(L, 2));
    gen_IR(Label, new_value(L, 1));
    gen_IR(Assign, new_value(V, 1), new_value(Const, 2));
    // gen_IR(Goto, new_value(L, 3));
    gen_IR(Write, new_value(V, 1));
    gen_IR(Label, new_value(L, 2));
    gen_IR(Calculate, new_value(V, 1), new_value(V, 2), new_value(V, 3), "*");
}