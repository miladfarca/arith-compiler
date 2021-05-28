#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "flags.h"
#include "codegen.h"

//globals
unsigned char *codegen_mem = NULL;
int codegen_mem_offset = 0;

// helpers
fpr allocated_fprs[FPR_COUNT] = {0};
void init_codegen()
{
    //reserve executable memory and init offset
    int size = 1024;
    codegen_mem_offset = 0;
    codegen_mem = (unsigned char *)mmap(0, size,
                                        PROT_READ | PROT_WRITE | PROT_EXEC,
                                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //set reserved regs
    int i;
    for (i = 0; i < sizeof(reserved_fprs) / sizeof(reserved_fprs[0]); i++)
    {
        allocated_fprs[reserved_fprs[i]] = -1;
    }
}
int run_codegen_and_return(fpr reg_result)
{
    prepare_return(reg_result);
    dealocate_fpr(reg_result);
    int (*fun_ptr)() = (void *)codegen_mem;
    return (*fun_ptr)();
}

// reg allocation
fpr allocate_fpr()
{
    // loop all listed fprs. If any is not allocated
    // then mark it as allocated and return it
    int i;
    for (i = 0; i < FPR_COUNT; i++)
    {
        // we need to match the first arch register to index 0
        // of our allocated_registers, check if there is an offset
        if (allocated_fprs[i] != 1 && allocated_fprs[i] != -1)
        {
            if (flag__print_reg_alloc)
            {
                printf("allocated %s\n", get_fpr_symbol(i));
            }
            allocated_fprs[i] = 1;
            return i;
        }
    }
    printf("no more registers!\n");
    return -1;
}
void dealocate_fpr(fpr reg)
{
    // check if it's a reserved regsiter
    if (allocated_fprs[reg] != -1)
    {
        allocated_fprs[reg] = 0;
        if (flag__print_reg_alloc)
        {
            printf("de-allocated %s\n", get_fpr_symbol(reg));
        }
    }
}

// debuging
char *get_gpr_symbol(gpr reg)
{
    return gpr_order[reg];
}
char *get_fpr_symbol(fpr reg)
{
    return fpr_order[reg];
}
void print_inst(char *instr_symbol, int imm, char *reg_dst, char *reg_src, char *comments)
{
    if (flag__print_code)
    {
        if (reg_dst == NULL && reg_src == NULL)
        {
            // instruction has no operands
            printf("%-4s %s\n", instr_symbol, comments);
        }
        else if (reg_src == NULL)
        {
            // immediate insted of source
            printf("%-4s %d %s %s\n", instr_symbol, imm, reg_dst, comments);
        }
        else
        {
            printf("%-4s %s %s %s\n", instr_symbol, reg_src, reg_dst, comments);
        }
    }
}

// opcodes
#define ARCH_OPCODES(arch)                               \
    void load_int_to_fpr(int imm, fpr reg_dst)           \
    {                                                    \
        load_int_to_fpr_##arch(imm, reg_dst);            \
    }                                                    \
    void negate_fpr(fpr reg_dst)                         \
    {                                                    \
        negate_fpr_##arch(reg_dst);                      \
    }                                                    \
    void add_fpr_to_fpr(fpr reg_dst, fpr reg_src)        \
    {                                                    \
        add_fpr_to_fpr_##arch(reg_dst, reg_src);         \
    }                                                    \
    void subtract_fpr_from_fpr(fpr reg_dst, fpr reg_src) \
    {                                                    \
        subtract_fpr_from_fpr_##arch(reg_dst, reg_src);  \
    }                                                    \
    void multiply_fpr_to_fpr(fpr reg_dst, fpr reg_src)   \
    {                                                    \
        multiply_fpr_to_fpr_##arch(reg_dst, reg_src);    \
    }                                                    \
    void divide_fpr_by_fpr(fpr reg_dst, fpr reg_src)     \
    {                                                    \
        divide_fpr_by_fpr_##arch(reg_dst, reg_src);      \
    }                                                    \
    void prepare_return(fpr reg_result)                  \
    {                                                    \
        prepare_return_##arch(reg_result);               \
    }

#if defined(_M_X64) || defined(__x86_64__)
// x64
ARCH_OPCODES(x64)
#elif defined(__AARCH64EL__) || defined(_M_ARM64)
// ARM64
#elif defined(__mips64)
// MIPS64
#elif defined(__PPC64__) || defined(_ARCH_PPC64)
// PPC64
#elif defined(__s390__) || defined(__s390x__)
// S390x
#else
#error "Unknown architecture!"
#endif
