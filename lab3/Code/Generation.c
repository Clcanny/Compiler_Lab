#include "list.h"
#include "Generation.h"
#include <stdarg.h>
#include <stdio.h>

Value* new_value(int kind, int value) 
{
    Value *new_value = (Value*)malloc(sizeof(Value));
    new_value->kind = kind;
    if(kind == 4)
        new_value->u.value = value;
    else
        new_value->u.no = value;
    char *value_str = (char*)malloc(sizeof(char) * 10);
    switch(kind)
    {
        case 0:
            sprintf(value_str, "v%d", value);
            break;
        case 1:
            sprintf(value_str, "t%d", value);
            break;
        case 2:
            sprintf(value_str, "l%d", value);
            break;
        case 3:
            sprintf(value_str, "f%d", value);
            break;
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

list_t *new_IR_list()
{
    list_t *new_list = list_new(free_IR);
    return new_list;
}

void del_IR_list()
{
    list_destroy(IR_list);
}

void traverse_IR_list(void (*action)(IR*))
{
    list_node_t *node;
    list_iterator_t *it = list_iterator_new(IR_list, LIST_HEAD);
    while ((node = list_iterator_next(it))) 
    {
        action((IR*)node->val);
    }
    list_iterator_destroy(it);
}

void print_IR(IR *ir)
{
    switch(ir->kind)
    {
        case 0:
            printf("LABEL %s :\n", ir->target->str);
            break;
        case 1:
            printf("FUNCTION %s :\n", ir->target->str);
            break;
        case 2:
            printf("%s := %s %s %s\n", ir->target->str, 
                ir->arg1->str, ir->u.op, ir->arg2->str);
            break;
        case 3:
            printf("%s := %s\n", ir->target->str, ir->arg1->str);
            break;
        case 4:
            printf("GOTO %s\n", ir->target->str);
            break;
        case 5:
            printf("IF %s %s %s GOTO %s\n", ir->arg1->str,
                ir->u.relop, ir->arg2->str, ir->target->str);
            break;
        case 6:
            printf("RETURN %s\n", ir->target->str);
            break;
        case 7:
            printf("DEC %s %d\n", ir->target->str, ir->arg1->u.value);
            break;
        case 8:
            printf("ARG %s\n", ir->target->str);
            break;
        case 9:
            printf("%s := CALL %s\n", ir->target->str, ir->arg1->str);
            break;
        case 10:
            printf("PARAM %s\n", ir->target->str);
            break;
        case 11:
            printf("READ %s\n", ir->target->str);
            break;
        case 12:
            printf("WRITE %s\n", ir->target->str);
            break;
    }
}