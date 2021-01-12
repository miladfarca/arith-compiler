#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "../codegen.h"

// register allocation
gpr scratch_gpr = ecx;
fpr scratch_fpr = xmm0;
// list of fp registers that should not be used directly
fpr reserved_fprs[1] = {xmm0};
char *gpr_order[GPR_COUNT] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
char *fpr_order[FPR_COUNT] = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6",
                              "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13",
                              "xmm14", "xmm15"};

#define PREP_REG_CODE()                      \
    unsigned char reg_code = 0xc0;           \
    /* add dst */                            \
    reg_code |= (unsigned char)reg_dst << 3; \
    /* add src */                            \
    reg_code |= (unsigned char)reg_src;

void load_int_to_gpr_x64(int imm, gpr reg_dst)
{
    // MOV
    gpr reg_code = (unsigned char) 0xb8 | reg_dst; 
    emit(reg_code);
    emit_imm(imm);
    print_inst("movl", imm, get_gpr_symbol(reg_dst), NULL, "\t\t# move immediate to general register");
}

void load_int_to_fpr_x64(int imm, fpr reg_dst)
{
    // first move it to a gpr
    load_int_to_gpr_x64(imm, scratch_gpr);
    // now move it to fpr (convert to float)
    convert_gpr_to_fpr_x64(reg_dst, scratch_gpr);
}

void convert_gpr_to_fpr_x64(fpr reg_dst, gpr reg_src)
{
    // CVTSI2SS
    PREP_REG_CODE()
    emit(0xf3);
    emit(0x0f);
    emit(0x2a);
    emit(reg_code);
    print_inst("cvtsi2ss", 0, get_fpr_symbol(reg_dst), get_gpr_symbol(reg_src), "\t# convert integer to floating point");
}

void convert_fpr_to_gpr_x64(gpr reg_dst, fpr reg_src)
{
    // CVTTSS2SI
    PREP_REG_CODE()
    emit(0xf3);
    emit(0x0f);
    emit(0x2c);
    emit(reg_code);
    print_inst("cvttss2si", 0, get_gpr_symbol(reg_dst), get_fpr_symbol(reg_src), "\t# convert floating point to integer");
}

void negate_fpr_x64(fpr reg_dst)
{
    convert_fpr_to_gpr_x64(scratch_gpr, reg_dst);
    // NEG
    unsigned char reg_code = 0xd8;
    // add dst
    reg_code |= (unsigned char)scratch_gpr;
    emit(0xf7);
    emit(reg_code);
    print_inst("neg", 0, get_gpr_symbol(scratch_gpr), get_gpr_symbol(scratch_gpr), "\t\t# get the two's complement");
    convert_gpr_to_fpr_x64(reg_dst, scratch_gpr);
}

void add_fpr_to_fpr_x64(fpr reg_dst, fpr reg_src)
{
    // ADDSS
    PREP_REG_CODE()
    emit(0xf3);
    emit(0x0f);
    emit(0x58);
    emit(reg_code);
    print_inst("addss", 0, get_fpr_symbol(reg_dst), get_fpr_symbol(reg_src), "\t# add fp register to fp register");
}

void subtract_fpr_from_fpr_x64(fpr reg_dst, fpr reg_src)
{
    // SUBSS
    PREP_REG_CODE()
    emit(0xf3);
    emit(0x0f);
    emit(0x5c);
    emit(reg_code);
    print_inst("subss", 0, get_fpr_symbol(reg_dst), get_fpr_symbol(reg_src), "\t# subtract fp register from fp register");
}

void multiply_fpr_to_fpr_x64(fpr reg_dst, fpr reg_src)
{
    // MULSS
    PREP_REG_CODE()
    emit(0xf3);
    emit(0x0f);
    emit(0x59);
    emit(reg_code);
    print_inst("mulss", 0, get_fpr_symbol(reg_dst), get_fpr_symbol(reg_src), "\t# multiply fp register to fp register");
}

void divide_fpr_by_fpr_x64(fpr reg_dst, fpr reg_src)
{
    // DIVSS
    PREP_REG_CODE()
    emit(0xf3);
    emit(0x0f);
    emit(0x5E);
    emit(reg_code);
    print_inst("divss", 0, get_fpr_symbol(reg_dst), get_fpr_symbol(reg_src), "\t# divide fp register by fp register");
}

void prepare_return_x64()
{
    // return register is eax
    convert_fpr_to_gpr_x64(eax, final_destination);
    // retq
    emit(0xc3);
}
