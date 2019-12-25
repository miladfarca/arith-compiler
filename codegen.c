#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "codegen.h"

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
            printf("allocated %d\n", i);
            allocated_registers[i] = 1;
            return i + FIRST_REG;
        }
    }
    printf("no more registers!\n");
    return -1;
}
void dealocate_reg(a_register reg)
{
    //check if it's a forbidden regsiter
    if (allocated_registers[reg - FIRST_REG] != -1)
    {
        allocated_registers[reg - FIRST_REG] = 0;
        printf("de-allocated %d\n", reg);
    }
}
void set_final_destination(a_register reg)
{
    final_destination = reg;
}

//opcodes
#define ARCH_OPCODES(arch)                                                   \
    void load_int_to_register(int imm, a_register reg)                       \
    {                                                                        \
        load_int_to_register_##arch(imm, reg);                               \
    }                                                                        \
    void move_register_to_register(a_register reg_0, a_register reg_1)       \
    {                                                                        \
        move_register_to_register_##arch(reg_0, reg_1);                      \
    }                                                                        \
    void add_register_to_register(a_register reg_0, a_register reg_1)        \
    {                                                                        \
        add_register_to_register_##arch(reg_0, reg_1);                       \
    }                                                                        \
    void subtract_register_from_register(a_register reg_0, a_register reg_1) \
    {                                                                        \
        subtract_register_from_register_##arch(reg_0, reg_1);                \
    }                                                                        \
    void multiply_register_to_register(a_register reg_0, a_register reg_1)   \
    {                                                                        \
        multiply_register_to_register_##arch(reg_0, reg_1);                  \
    }                                                                        \
    void divide_register_by_register(a_register reg_0, a_register reg_1)     \
    {                                                                        \
        divide_register_by_register_##arch(reg_0, reg_1);                    \
    }                                                                        \
    void prepare_return()                                                    \
    {                                                                        \
        prepare_return_##arch();                                             \
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