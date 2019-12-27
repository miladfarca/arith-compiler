#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "flags.h"
#include "codegen.h"

//globals
void *codegen_mem = NULL;
int codegen_mem_offset = 0;

// helpers
a_register allocated_registers[REG_COUNT] = {0};
void init_codegen()
{
    //reserve memory and init offset
    int size = 1024;
    codegen_mem_offset = 0;
    codegen_mem = mmap(0, size,
                       PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //set forbidden regs
    int i;
    for (i = 0; i < sizeof(forbidden_registers) / sizeof(forbidden_registers[0]); i++)
    {
        allocated_registers[forbidden_registers[i]] = -1;
    }
}
void emit(unsigned char byte)
{
    memcpy(codegen_mem + codegen_mem_offset, &byte, 1);
    codegen_mem_offset++;
}
int run_codegen_and_return()
{
    prepare_return();
    int (*fun_ptr)() = codegen_mem;
    return (*fun_ptr)();
}

// reg allocation
a_register get_free_register()
{
    // loop all listed regs. If any is not allocated
    // then mark it as allocated and return it
    int i;
    for (i = 0; i <= REG_COUNT - 1; i++)
    {
        // we need to match the first arch register to index 0
        // of our allocated_registers, check if there is an offset
        if (allocated_registers[i] != 1 && allocated_registers[i] != -1)
        {
            if (flag__print_reg_aloc)
            {
                printf("> allocated %s\n", get_reg_symbol(i));
            }
            allocated_registers[i] = 1;
            return i + first_reg;
        }
    }
    printf("no more registers!\n");
    return -1;
}
void dealocate_reg(a_register reg)
{
    //check if it's a forbidden regsiter
    if (allocated_registers[reg - first_reg] != -1)
    {
        allocated_registers[reg - first_reg] = 0;
        if (flag__print_reg_aloc)
        {
            printf("> de-allocated %s\n", get_reg_symbol(reg));
        }
    }
}
a_register final_destination = -1;
void set_final_destination(a_register reg)
{
    final_destination = reg;
}

//debuging
char *get_reg_symbol(a_register reg)
{
    return register_order[reg];
}
void print_inst(char *instr_symbol, int imm, a_register reg_dst, a_register reg_src, int has_imm_input)
{
    if (flag__print_code)
    {
        if (has_imm_input)
        {
            printf("%-4s %s %d\n", instr_symbol, get_reg_symbol(reg_dst), imm);
        }
        else
        {
            printf("%-4s %s %s\n", instr_symbol, get_reg_symbol(reg_dst), get_reg_symbol(reg_src));
        }
    }
}

//opcodes
#define ARCH_OPCODES(arch)                                                       \
    void load_int_to_register(int imm, a_register reg_dst)                       \
    {                                                                            \
        load_int_to_register_##arch(imm, reg_dst);                               \
    }                                                                            \
    void move_register_to_register(a_register reg_dst, a_register reg_src)       \
    {                                                                            \
        move_register_to_register_##arch(reg_dst, reg_src);                      \
    }                                                                            \
    void add_register_to_register(a_register reg_dst, a_register reg_src)        \
    {                                                                            \
        add_register_to_register_##arch(reg_dst, reg_src);                       \
    }                                                                            \
    void subtract_register_from_register(a_register reg_dst, a_register reg_src) \
    {                                                                            \
        subtract_register_from_register_##arch(reg_dst, reg_src);                \
    }                                                                            \
    void multiply_register_to_register(a_register reg_dst, a_register reg_src)   \
    {                                                                            \
        multiply_register_to_register_##arch(reg_dst, reg_src);                  \
    }                                                                            \
    void divide_register_by_register(a_register reg_dst, a_register reg_src)     \
    {                                                                            \
        divide_register_by_register_##arch(reg_dst, reg_src);                    \
    }                                                                            \
    void prepare_return()                                                        \
    {                                                                            \
        prepare_return_##arch();                                                 \
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