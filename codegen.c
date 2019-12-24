#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "codegen.h"

#define reg_count 8

//arch specific
// register allocation
a_register first_reg = eax;
// list of registers that should not be used directly
a_register forbidden_registers[] = {edx, esp, ebp};
a_register allocated_registers[reg_count] = {0};

// helpers
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
void set_final_destination(a_register reg)
{
    final_destination = reg;
}

// reg allocation
a_register get_free_register()
{
    // loop all listed regs. If any is not allocated
    // then mark it as allocated and return it
    int i;
    for (i = 0; i <= reg_count - 1; i++)
    {
        // we need to match the first arch register to index 0
        // of our allocated_registers, check if there is an offset
        if (allocated_registers[i] != 1 && allocated_registers[i] != -1)
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
    //check if it's a forbidden regsiter
    if (allocated_registers[reg - first_reg] != -1)
    {
        allocated_registers[reg - first_reg] = 0;
        printf("de-allocated %d\n", reg);
    }
}

// opcodes
//TODO: explain format and how code is emitted
//TODO: how is endianness effecting this
void load_int_to_register(int imm, a_register reg)
{
    //TODO: wy is the reg code calc diff from others
    // movl $imm, %reg
    emit((char)(0xb8 + reg));
    emit((char)imm);
    emit(0x00);
    emit(0x00);
    emit(0x00);
}
void move_register_to_register(a_register reg_0, a_register reg_1)
{
    // dst = reg_0
    // mov %reg_0, %reg_1
    // format: 89 /r
    unsigned char reg_code = 0xc0;
    // add reg_0
    reg_code |= (unsigned char)reg_1 << 3;
    // add reg_1
    reg_code |= (unsigned char)reg_0;
    emit(0x89);
    emit(reg_code);
}
void add_register_to_register(a_register reg_0, a_register reg_1)
{
    // dst = reg_0
    // add $reg_0, %reg_1
    // format: 01 /r
    // r : 1 1 (_ _ _) (_ _ _) = 1 1 reg_1 reg_0
    unsigned char reg_code = 0xc0;
    // add reg_0
    reg_code |= (unsigned char)reg_1 << 3;
    // add reg_1
    reg_code |= (unsigned char)reg_0;
    emit(0x01);
    emit(reg_code);
}

void subtract_register_from_register(a_register reg_0, a_register reg_1)
{
    // dst = reg_0
    // sub $ebx, %eax
    // format: 29 /r
    unsigned char reg_code = 0xc0;
    // add reg_0
    reg_code |= (unsigned char)reg_1 << 3;
    // add reg_1
    reg_code |= (unsigned char)reg_0;
    emit(0x29);
    emit(reg_code);
}

void multiply_register_to_register(a_register reg_0, a_register reg_1)
{
    //TODO: why the reg order differs from add?
    // format:  0F AF /r
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

void divide_register_by_register(a_register reg_0, a_register reg_1)
{
    // dest = reg_0
    // we will have to use eax as src and dest reg while computing
    // format: F7 /7
    // idiv %reg
    printf("r0=%d\n", reg_0);
    printf("r1=%d\n", reg_1);
    move_register_to_register(eax, reg_0);
    emit(0xf7);
    emit((char)(0xf8 + reg_1));
    move_register_to_register(reg_0, eax);
}

void prepare_return()
{
    //return register is eax
    move_register_to_register(eax, final_destination);
    // retq
    emit(0xc3);
}