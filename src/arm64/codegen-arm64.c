#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "../flags.h"
#include "../codegen.h"

// register allocation
gpr scratch_gpr = w1;
fpr scratch_fpr = s0;
// list of fp registers that should not be used directly
fpr reserved_fprs[1] = {s0};
char *gpr_order[GPR_COUNT] = {"w0", "w1", "w2", "w3", "w4", "w5", "w6", "w6"};
char *fpr_order[FPR_COUNT] = {"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
                              "s8", "s9", "s10", "s11", "s12", "s13", "s14", "s15"};
// debuging
char *get_gpr_symbol(gpr reg)
{
    return gpr_order[reg];
}
char *get_fpr_symbol(fpr reg)
{
    return fpr_order[reg];
}
void print_inst(char *instr_symbol, char *instr_symbol_intermediate, int imm, char *reg_dst, char *reg_src1, char *reg_src2, char *comments)
{
    if (flag__print_code)
    {
        if (reg_dst == NULL && reg_src1 == NULL)
        {
            // instruction has no operands.
            printf("%-4s %s\n", instr_symbol, comments);
        }
        else if (reg_src1 == NULL)
        {
            if (instr_symbol_intermediate == NULL)
            {
                //immediate insted of source.
                printf("%-4s %s %d %s\n", instr_symbol, reg_dst, imm, comments);
            }
            else
            {
                // two instrctions.
                printf("%-4s %s %d %s %s\n", instr_symbol, reg_dst, imm, instr_symbol_intermediate, comments);
            }
        }
        else
        {
            if (reg_src2 == NULL)
            {
                printf("%-4s %s %s %s\n", instr_symbol, reg_dst, reg_src1, comments);
            }
            else
            {
                printf("%-4s %s %s %s %s\n", instr_symbol, reg_dst, reg_src1, reg_src2, comments);
            }
        }
    }
}

// code emitters
void emit(unsigned int instruction)
{
    *(int *)(codegen_mem + codegen_mem_offset) = instruction;
    codegen_mem_offset += 4;
}

void load_int_to_gpr_arm64(int imm, gpr reg_dst)
{
    // mov
    unsigned int imm_high = (unsigned int)imm >> 16;
    unsigned int imm_low = imm & 0xFFFF;
    emit(0x694 << 21 | imm_low << 5 | (reg_dst & 0x1F));
    print_inst("mov", NULL, imm_low, get_gpr_symbol(reg_dst), NULL, NULL, "\t\t# move immediate to general register");
    // movk
    emit(0x795 << 21 | imm_high << 5 | (reg_dst & 0x1F));
    print_inst("movk", "lsl 16", imm_high, get_gpr_symbol(reg_dst), NULL, NULL, "");
}

void load_int_to_fpr_arm64(int imm, fpr reg_dst)
{
    // first move it to a gpr
    load_int_to_gpr_arm64(imm, scratch_gpr);
    // now move it to fpr (convert to float)
    convert_gpr_to_fpr_arm64(reg_dst, scratch_gpr);
}

void convert_gpr_to_fpr_arm64(fpr reg_dst, gpr reg_src)
{
    // scvtf
    emit(0x78880 << 10 | reg_src << 5 | (reg_dst & 0x1F));
    print_inst("scvtf", NULL, 0, get_fpr_symbol(reg_dst), get_gpr_symbol(reg_src), NULL, "\t\t# convert integer to floating point");
}

void convert_fpr_to_gpr_arm64(gpr reg_dst, fpr reg_src)
{
    // fcvtzs
    emit(0x78E00 << 10 | reg_src << 5 | (reg_dst & 0x1F));
    print_inst("fcvtzs", NULL, 0, get_gpr_symbol(reg_dst), get_fpr_symbol(reg_src), NULL, "\t\t# convert floating point to integer");
}

void negate_fpr_arm64(fpr reg_dst)
{
    // fneg
    emit(0x78850 << 10 | (reg_dst & 0x1F) << 5 | (reg_dst & 0x1F));
    print_inst("fneg", NULL, 0, get_gpr_symbol(scratch_gpr), get_gpr_symbol(scratch_gpr), NULL, "\t\t# get the two's complement");
}

void add_fpr_to_fpr_arm64(fpr reg_dst, fpr reg_src)
{
    // fadd
    emit(0xF1 << 21 | (reg_src & 0x1F) << 16 | 0xA << 10 | (reg_dst & 0x1F) << 5 | (reg_dst & 0x1F));
    print_inst("fadd", NULL, 0, get_fpr_symbol(reg_dst), get_fpr_symbol(reg_dst), get_fpr_symbol(reg_src), "\t\t# add fp register to fp register");
}

void subtract_fpr_from_fpr_arm64(fpr reg_dst, fpr reg_src)
{
    // fsub
    emit(0xF1 << 21 | (reg_src & 0x1F) << 16 | 0xE << 10 | (reg_dst & 0x1F) << 5 | (reg_dst & 0x1F));
    print_inst("fsub", NULL, 0, get_fpr_symbol(reg_dst), get_fpr_symbol(reg_dst), get_fpr_symbol(reg_src), "\t\t# subtract fp register from fp register");
}

void multiply_fpr_to_fpr_arm64(fpr reg_dst, fpr reg_src)
{
    // fmul
    emit(0xF1 << 21 | (reg_src & 0x1F) << 16 | 0x2 << 10 | (reg_dst & 0x1F) << 5 | (reg_dst & 0x1F));
    print_inst("fmul", NULL, 0, get_fpr_symbol(reg_dst), get_fpr_symbol(reg_dst), get_fpr_symbol(reg_src), "\t\t# multiply fp register to fp register");
}

void divide_fpr_by_fpr_arm64(fpr reg_dst, fpr reg_src)
{
    // fdiv
    emit(0xF1 << 21 | (reg_src & 0x1F) << 16 | 0x6 << 10 | (reg_dst & 0x1F) << 5 | (reg_dst & 0x1F));
    print_inst("fdiv", NULL, 0, get_fpr_symbol(reg_dst), get_fpr_symbol(reg_dst), get_fpr_symbol(reg_src), "\t\t# divide fp register by fp register");
}

void prepare_return_arm64(fpr reg_result)
{
    // return register is eax
    convert_fpr_to_gpr_arm64(w0, reg_result);
    // ret, address is in x30
    emit(0xd65f03c0);
}
