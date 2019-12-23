#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "codegen.h"

// helpers
void init_codegen()
{
    //reserve memory and init offset
    int size = 1024;
    codegen_mem_offset = 0;
    codegen_mem = mmap(0, size,
                       PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void emit(unsigned char byte)
{
    memcpy(codegen_mem + codegen_mem_offset, &byte, 1);
    codegen_mem_offset++;
}

//arch specific
// register allocation
a_register first_reg = eax;
const int reg_count = 4;
a_register allocated_registers[reg_count] = {0};
a_register get_free_register()
{
    // loop all listed regs. If any is not allocated
    // then mark it as allocated and return it
    int i;
    for (i = 0; i <= reg_count - 1; i++)
    {
        // we need to match the first arch register to index 0
        // of our allocated_registers, check if there is an offset
        if (allocated_registers[i] != 1)
        {
            printf("allocated %d\n", i);
            allocated_registers[i] = 1;
            return i + first_reg;
        }
    }
    printf("no more registers!\n");
    return -1;
}
void dealocate_reg(a_register reg)
{
    allocated_registers[reg - first_reg] = 0;
    printf("de-allocated %d\n", reg);
}

// opcodes
void load_int_to_register(int imm, a_register reg)
{
    // movl $imm, %reg
    emit((char)(0xb8 + reg));
    emit((char)imm);
    emit(0x00);
    emit(0x00);
    emit(0x00);
}
void add_register_to_register(a_register reg_0, a_register reg_1)
{
    // dst = reg_0
    // add $reg_0, %reg_1
    // 1 1 (_ _ _) (_ _ _) = 1 1 reg_1 reg_0
    unsigned char reg_code = 0xc0;
    // add reg_0
    reg_code |= (unsigned char)reg_1 << 3;
    // add reg_1
    reg_code |= (unsigned char)reg_0;
    emit(0x01);
    emit(reg_code);
}

void subtract_scratch_from_accum()
{
    // sub $ebx, %eax
    //emit(0x29);
    //emit(0xd8);
}

void multiply_register_to_register(a_register reg_0, a_register reg_1)
{
    //TODO: why the reg order differs from add?
    // dest = reg_0
    // imul %reg_0, %reg_1
    unsigned char reg_code = 0xc0;
    // add reg_0
    reg_code |= (unsigned char)reg_0 << 3;
    // add reg_1
    reg_code |= (unsigned char)reg_1;
    emit(0x0f);
    emit(0xaf);
    emit(reg_code);
}

void divide_accum_by_scratch()
{
    //
}
int run_codegen_and_return()
{
    // retq
    emit(0xc3);
    int (*fun_ptr)() = codegen_mem;
    return (*fun_ptr)();
}