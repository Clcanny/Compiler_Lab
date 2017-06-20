#include "MIPS_asm.h"
#include "Generation.h"

void start_gen_asm()
{
    asm_block_stack = (ASM_Block_Stack*)malloc(sizeof(ASM_Block_Stack));
    asm_block_stack->data = list_new(free_asm_block);
    asm_block_stack->top = NULL;
    p_asm(".data\n");
    asm_data_str(var_name("_prompt"), "Enter an integer:");
    asm_data_str(var_name("_ret"), "\\n");
    p_asm(".global main\n");
    p_asm(".text\n");
    p_asm("read:\n");
    asm_li(reg("v", 0), imm(4));
    asm_la(reg("a", 0), var_name("_prompt"));
    asm_sys();
    asm_li(reg("v", 0), imm(5));
    asm_sys();
    asm_return(reg_ra());
    p_asm("write:\n");
    asm_li(reg("v", 0), imm(1));
    asm_sys();
    asm_li(reg("v", 0), imm(4));
    asm_la(reg("a", 0), var_name("_ret"));
    asm_sys();
    asm_mv(reg("v", 0), reg_0());
    asm_return(reg_ra());

    list_node_t *node;
    list_iterator_t *it = list_iterator_new(IR_list, LIST_HEAD);
    while((node = list_iterator_next(it)))
    {
        genAsm(node);
    }
}

void end_gen_asm()
{
    list_destroy(asm_block_stack->data);
    free(asm_block_stack);
}

void free_asm_block(void *block)
{
    if(block == NULL)
        return;
    ASM_Block *asm_block = (ASM_Block*)block;
    if(asm_block->var_list == NULL) {
        free(asm_block);
        return;
    }
    list_node_t *node;
    list_iterator_t *it = list_iterator_new(asm_block->var_list, LIST_HEAD);
    VarInfo *var;
    while((node = list_iterator_next(it)))
    {
        var = (VarInfo*)(node->val);
        free(var);
    }
    list_iterator_destroy(it);
    free(asm_block);
}

void free_var_info(void *info)
{
    if(info == NULL)
        return;
    VarInfo *var = (VarInfo*)info;
    free(var);
}

ASM_Block *push_asm_block(int offset)
{
    ASM_Block *block = (ASM_Block*)malloc(sizeof(ASM_Block));
    block->var_list = list_new(free_var_info);
    block->basis = offset;
    block->offset = 0;
    list_rpush(asm_block_stack->data, list_node_new(block));
    asm_block_stack->top = block;
}

void pop_asm_block()
{
    if(asm_block_stack->top == NULL)
        return;
    list_remove(asm_block_stack->data, asm_block_stack->data->tail);
    if(asm_block_stack->data->len == 0)
        asm_block_stack->top = NULL;
    else
        asm_block_stack->top = (ASM_Block*)(asm_block_stack->data->tail->val);
}

VarInfo* add_var(int size, int no)
{
    ASM_Block *block = asm_block_stack->top;
    if(block == NULL)
        return NULL;
    VarInfo *var = (VarInfo*)malloc(sizeof(VarInfo));
    var->no = no;
    block->offset += size;
    var->offset = block->offset;
    list_rpush(block->var_list, list_node_new(var));
    return var;
}

VarInfo *find_var(int no, ASM_Block **block)
{
    list_node_t *node_var, *node_block;
    VarInfo *var;
    ASM_Block *cur_block;
    list_iterator_t *it_var, *it_block = 
        list_iterator_new(asm_block_stack->data, LIST_HEAD);
    while((node_block = list_iterator_next(it_block)))
    {
        cur_block = (ASM_Block*)(node_block->val);
        it_var = list_iterator_new(cur_block->var_list, LIST_HEAD);
        while((node_var = list_iterator_next(it_var)))
        {
            var = (VarInfo*)(node_var->val);
            if(var->no == no) {
                *block = cur_block;
                return var;
            }
        }
        list_iterator_destroy(it_var);      
    }
    list_iterator_destroy(it_block);
    return NULL;
}
