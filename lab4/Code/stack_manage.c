#include <string.h>
#include "stack_manage.h"
#include "Generation.h"
#include "MIPS_asm.h"

void prepare(Value *value, int num)
{
    if(value == NULL)
        return;
    if(value->kind == Const) {
        asm_li(reg("t", num), imm(value->u.value));
        return;
    }
    ASM_Block *block = NULL;
    VarInfo *var = find_var(value->u.no, &block);
    if(var == NULL)
        return;
    int offset = -(block->basis + var->offset);
    if(value->kind == V) {
        asm_lw(reg("t", num), addr_im_reg(imm(offset), reg_fp()));
    }
    else if(value->kind == Address) {
        asm_la(reg("t", num), addr_im_reg(imm(offset), reg_fp()));
    }
    else if(value->kind == Content) {
        asm_lw(reg("t", num), addr_im_reg(imm(offset), reg_fp()));
        asm_lw(reg("t", num), addr_reg(reg("t", num)));
    }
}

void save(Value *value, int num)
{
    if(value == NULL)
        return;
    ASM_Block *block = NULL;
    VarInfo *var = find_var(value->u.no, &block);
    if(var == NULL) {
        var = add_var(4, value->u.no);
        block = asm_block_stack->top;
    }
    if(block == NULL)
        return;
    int offset = -(block->basis + var->offset);
    if(value->kind == V) {
        asm_sw(reg("t", num), addr_im_reg(imm(offset), reg_fp()));
    }
    else if(value->kind == Content) {
        asm_lw(reg("t", 3), addr_im_reg(imm(offset), reg_fp()));
        asm_sw(reg("t", num), addr_reg(reg("t", 3)));
    }

}

void genAsm(list_node_t *node)
{
    IR *ir = (IR *)(node->val);
    switch(ir->kind)
    {
        case Label:
            p_asm("l%d:\n", ir->target->u.no);
            break;
        case Fun:
            pop_asm_block();
            if(ir->target->u.no == 0) {
                p_asm("main:\n");
                asm_mv(reg_fp(), reg_sp());
                asm_sw(reg_fp(), addr_im_reg(imm(-4), reg_fp()));
                asm_sub(reg_sp(), reg_sp(), imm(8));
            }
            else {
                p_asm("f%d:\n", ir->target->u.no);
            }
            push_asm_block(8);
            init_var_list(node);
            break;
        case Return:
            asm_mv(reg_sp(), reg_fp());
            // asm_lw(reg_fp(), addr_im_reg(imm(-4), reg_sp()));
            prepare(ir->target, 0);
            asm_lw(reg_fp(), addr_im_reg(imm(-4), reg_sp()));
            asm_mv(reg("v", 0), reg("t", 0));
            asm_return(reg_ra());
            break;
        case Call:
            asm_sw(reg_fp(), addr_im_reg(imm(-4), reg_sp()));
            // asm_sw(reg_ra(), addr_im_reg(imm(-8), reg_sp()));
            list_node_t *tmp = node->prev;
            int i = 1;
            while (((IR *)tmp->val)->kind == Arg)
            {
                prepare(((IR *)tmp->val)->target, 0);
                asm_sw(reg("t", 0), addr_im_reg(imm(-8 - i * 4), reg_sp()));
                tmp = tmp->prev;
                i++;
            }
            asm_sw(reg_ra(), addr_im_reg(imm(-8), reg_fp()));
            asm_mv(reg_fp(), reg_sp());
            asm_sub(reg_sp(), reg_sp(), imm(4 * (i + 1)));
            p_asm("jal f%d\n", ir->arg1->u.no);
            save(ir->target, 0);
            asm_lw(reg_ra(), addr_im_reg(imm(-8), reg_fp()));
            break;
        case Calculate:
            prepare(ir->arg1, 0);
            prepare(ir->arg2, 1);
            if(!strcmp(ir->u.op, "+"))
                asm_add(reg("t", 2), reg("t", 0), reg("t", 1));
            else if(!strcmp(ir->u.op, "-"))
                asm_sub(reg("t", 2), reg("t", 0), reg("t", 1));
            else if(!strcmp(ir->u.op, "*"))
                asm_mul(reg("t", 2), reg("t", 0), reg("t", 1));
            else if(!strcmp(ir->u.op, "/"))
                asm_div(reg("t", 2), reg("t", 0), reg("t", 1));
            /* asm_sw(reg("t", 2), ir->target->u.value); */
            save(ir->target, 2);
            break;
        case Assign:
            prepare(ir->arg1, 1);
            asm_mv(reg("t", 0), reg("t", 1));
            save(ir->target, 0);
            break;
        case Goto:
            asm_j(ir->target->u.value);
            break;
        case GotoRel:
            prepare(ir->arg1, 0);
            prepare(ir->arg2, 1);
            if (!strcmp(ir->u.relop, ">="))
            {
                asm_more_equal(reg("t", 0), reg("t", 1), ir->target->u.value);
            }
            else if (!strcmp(ir->u.relop, ">"))
            {
                asm_more(reg("t", 0), reg("t", 1), ir->target->u.value);
            }
            else if (!strcmp(ir->u.relop, "<="))
            {
                asm_less_equal(reg("t", 0), reg("t", 1), ir->target->u.value);
            }
            else if (!strcmp(ir->u.relop, "<"))
            {
                asm_less(reg("t", 0), reg("t", 1), ir->target->u.value);
            }
            else if (!strcmp(ir->u.relop, "=="))
            {
                asm_equal(reg("t", 0), reg("t", 1), ir->target->u.value);
            }
            else if (!strcmp(ir->u.relop, "!="))
            {
                asm_not_equal(reg("t", 0), reg("t", 1), ir->target->u.value);
            }
            break;
        case Read:
            p_asm("li $v0, 5\n");
            asm_sys();
            asm_mv(reg("t", 0), reg("v", 0));
            save(ir->target, 0);
            break;
        case Write:
            prepare(ir->target, 0);
            asm_mv(reg("a", 0), reg("t", 0));
            p_asm("li $v0, 1\n");
            asm_sys();
            p_asm("li $v0, 4\n");
            asm_la(reg("a", 0), var_name("_ret"));
            asm_sys();
            asm_mv(reg("v", 0), reg_0());
            break;
    }
}

void init_var_list(list_node_t *node)
{
    IR *ir = (IR*)(node->val);
    int size_sum = 0;
    while(node->next != NULL && ((IR*)(node->next->val))->kind != Fun)
    {
        if(ir->kind == Assign || ir->kind == Calculate || ir->kind == Dec
            || ir->kind == Call || ir->kind == Read) 
        {
            ASM_Block *block;
            if(find_var(ir->target->u.no, &block) != NULL) {
                node = node->next;
                ir = (IR*)(node->val);
                continue;
            }
            int size = 4;
            if(ir->kind == Dec)
                size = ir->arg1->u.value;
            add_var(size, ir->target->u.no);
            size_sum += size;
        }
        else if(ir->kind == Param)
        {
            add_var(4, ir->target->u.no);
        }
        node = node->next;
        ir = (IR*)(node->val);
    }
    asm_sub(reg_sp(), reg_sp(), imm(size_sum));
}