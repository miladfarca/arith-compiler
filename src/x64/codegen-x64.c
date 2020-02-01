#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "../codegen.h"

// register allocation
a_register first_reg = eax;
a_register scratch_reg = edi;
// list of registers that should not be used directly
a_register reserved_registers[4] = {edx, esp, ebp, edi};
char *register_order[REG_COUNT] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};

// opcodes
//TODO: explain format and how code is emitted
//TODO: how is endianness effecting this
void load_int_to_register_x64(int imm, a_register reg_dst)
{
    //TODO: wy is the reg code calc diff from others
    // movl $imm, %reg_dst
    emit((char)(0xb8 + reg_dst));
    emit((char)imm);
    emit(0x00);
    emit(0x00);
    emit(0x00);
    print_inst("mov", imm, reg_dst, no_reg);
}
void negate_register_x64(a_register reg_dst)
{
    // This operation is equivalent to subtracting the operand from 0
    load_int_to_register_x64(0, scratch_reg);
    subtract_register_from_register_x64(scratch_reg, reg_dst);
    move_register_to_register_x64(reg_dst, scratch_reg);
}
void move_register_to_register_x64(a_register reg_dst, a_register reg_src)
{
    if (reg_dst != reg_src)
    {
        // dst = reg_dst
        // mov %reg_dst, %reg_src
        // format: 89 /r
        unsigned char reg_code = 0xc0;
        // add reg_dst
        reg_code |= (unsigned char)reg_src << 3;
        // add reg_src
        reg_code |= (unsigned char)reg_dst;
        emit(0x89);
        emit(reg_code);
        print_inst("mov", -1, reg_dst, reg_src);
    }
}
void add_register_to_register_x64(a_register reg_dst, a_register reg_src)
{
    // dst = reg_dst
    // add $reg_dst, %reg_src
    // format: 01 /r
    // r : 1 1 (_ _ _) (_ _ _) = 1 1 reg_src reg_dst
    unsigned char reg_code = 0xc0;
    // add reg_dst
    reg_code |= (unsigned char)reg_src << 3;
    // add reg_src
    reg_code |= (unsigned char)reg_dst;
    emit(0x01);
    emit(reg_code);
    print_inst("add", -1, reg_dst, reg_src);
}

void subtract_register_from_register_x64(a_register reg_dst, a_register reg_src)
{
    // dst = reg_dst
    // sub $ebx, %eax
    // format: 29 /r
    unsigned char reg_code = 0xc0;
    // add reg_dst
    reg_code |= (unsigned char)reg_src << 3;
    // add reg_src
    reg_code |= (unsigned char)reg_dst;
    emit(0x29);
    emit(reg_code);
    print_inst("sub", -1, reg_dst, reg_src);
}

void multiply_register_to_register_x64(a_register reg_dst, a_register reg_src)
{
    //TODO: why the reg order differs from add?
    // format:  0F AF /r
    // dest = reg_dst
    // imul %reg_dst, %reg_src
    unsigned char reg_code = 0xc0;
    // add reg_dst
    reg_code |= (unsigned char)reg_dst << 3;
    // add reg_src
    reg_code |= (unsigned char)reg_src;
    emit(0x0f);
    emit(0xaf);
    emit(reg_code);
    print_inst("imul", -1, reg_dst, reg_src);
}

void divide_register_by_register_x64(a_register reg_dst, a_register reg_src)
{
    // dest = reg_dst
    // we will have to use eax:edx as src and dest reg while computing,
    // eax:edx means the first 32 bits are in EDX and the other half in EAX example for 4/2:
    // move eax, 4 -> cdq -> mov 2, ebx -> idiv ebx -> eax = 2 , edx = 0
    // format: F7 /7
    // idiv %reg
    // save eax content and restore later
    move_register_to_register(scratch_reg, eax);
    move_register_to_register(eax, reg_dst);
    //cdq , extend EAX to EDX
    emit(0x99);
    print_inst("cdq", -1, no_reg, no_reg);
    emit(0xf7);
    emit((char)(0xf8 + reg_src));
    print_inst("idiv", -1, reg_dst, reg_src);
    move_register_to_register(reg_dst, eax);
    move_register_to_register(eax, scratch_reg);
}

void prepare_return_x64()
{
    //return register is eax
    move_register_to_register(eax, final_destination);
    // retq
    emit(0xc3);
}