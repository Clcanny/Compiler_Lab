#ifndef MIP32_ASM_H
#define MIP32_ASM_H

#include <stdio.h>
#include "list.h"

#define p_asm(fmt, ...) printf(fmt, ##__VA_ARGS__)
// #define p_asm(fmt, ...) fprintf(f_output, fmt, ##__VA_ARGS__)

#define imm(val) p_asm("%d", val)
#define var_name(val) p_asm("%s", val)

#define reg_0() p_asm("$0")
#define reg_sp() p_asm("$sp")
#define reg_fp() p_asm("$fp")
#define reg_ra() p_asm("$ra")
#define reg(type, no) p_asm("$%s%d", type, no)

#define addr_im(val) p_asm("%d", val)
#define addr_reg(reg) p_asm("("),reg,p_asm(")")
#define addr_im_reg(imm, reg) imm,p_asm("("),reg,p_asm(")") 

#define asm_data_str(var_name, var) var_name,p_asm(": .asciiz \"%s\"\n", var)
#define asm_data_space(name, size) name,p_asm(": .space %d\n", size)

// add rd, rs, rt
// rd <-- rs + rt
#define asm_add(Rdest, Rsrc, rt) \
    p_asm("add "),Rdest,p_asm(", "),Rsrc,p_asm(", "),rt,p_asm("\n")
#define asm_sub(Rdest, Rsrc, rt) \
    p_asm("sub "),Rdest,p_asm(", "),Rsrc,p_asm(", "),rt,p_asm("\n")
#define asm_mul(Rdest, Rsrc, rt) \
    p_asm("mul "),Rdest,p_asm(", "),Rsrc,p_asm(", "),rt,p_asm("\n")
#define asm_div(Rdest, Rsrc, rt) \
    p_asm("div "),Rdest,p_asm(", "),Rsrc,p_asm(", "),rt,p_asm("\n")
#define asm_neg(Rdest, Rsrc) \
    p_asm("neg "),Rdest,p_asm(", -"),Rsrc
#define asm_and(Rdest, Rsrc, rt) \
    p_asm("and "),Rdest,p_asm(", "),Rsrc,p_asm(", "),rt,p_asm("\n")
#define asm_or(Rdest, Rsrc, rt) \
    p_asm("or "),Rdest,p_asm(", "),Rsrc,p_asm(", "),rt,p_asm("\n")

// sw rt, addr
// store the word from $rt to address
#define asm_sw(Reg, addr) p_asm("sw "),Reg,p_asm(", "),addr,p_asm("\n")
// lw rd, addr
// load the word (32 bit value) at address into $rd
#define asm_lw(Rdest, addr) p_asm("lw "),Rdest,p_asm(", "),addr,p_asm("\n")
// la rd, addr
// load the address into $rd
#define asm_la(Rdest, addr) p_asm("la "),Rdest,p_asm(", "),addr,p_asm("\n")
// li rd, imm
#define asm_li(Rdest, imm) p_asm("li "),Rdest,p_asm(", "),imm,p_asm("\n")
// move rd, rs
#define asm_mv(Rdest, Rsrc) p_asm("move "),Rdest,p_asm(", "),Rsrc,p_asm("\n")

#define asm_equal(Rs, Rt, no) \
    p_asm("beq "),Rs,p_asm(", "),Rt,p_asm(", "),p_asm("label%d\n", no)
#define asm_not_equal(Rs, Rt, no) \
    p_asm("bne "),Rs,p_asm(", "),Rt,p_asm(", "),p_asm("label%d\n", no)
#define asm_less(Rs, Rt, no) \
    p_asm("blt "),Rs,p_asm(", "),Rt,p_asm(", "),p_asm("label%d\n", no)
#define asm_less_equal(Rs, Rt, no) \
    p_asm("ble "),Rs,p_asm(", "),Rt,p_asm(", "),p_asm("label%d\n", no)
#define asm_more(Rs, Rt, no) \
    p_asm("bgt "),Rs,p_asm(", "),Rt,p_asm(", "),p_asm("label%d\n", no)
#define asm_more_equal(Rs, Rt, no) \
    p_asm("bge "),Rs,p_asm(", "),Rt,p_asm(", "),p_asm("label%d\n", no)
#define asm_j(no) \
    p_asm("j label%d\n", no)
#define asm_return(rt) \
    p_asm("jr "),rt,p_asm("\n")
#define asm_sys() \
    p_asm("syscall\n")

FILE *f_output;

typedef struct var_info
{
    int offset;
    int no;
} VarInfo;
void free_var_info(void *info);

typedef struct asm_block
{
    list_t *var_list;
    int basis;
    int offset;
} ASM_Block;
void free_asm_block(void *block);

typedef struct 
{
    list_t *data;
    ASM_Block *top;
} ASM_Block_Stack;
ASM_Block_Stack *asm_block_stack;

void start_gen_asm();
void end_gen_asm();
ASM_Block *push_asm_block(int offset);
void pop_asm_block();
VarInfo* add_var(int size, int no);
VarInfo *find_var(int no, ASM_Block **block);
void genAsm(list_node_t *node);

#endif
