#ifndef CODEGEN
#define CODEGEN
void *codegen_mem;
int codegen_mem_offset;

#if defined(_M_X64) || defined(__x86_64__)
// x64
#include "x64/codegen-x64.h"
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

//helpers
void init_codegen();
void emit(unsigned char byte);
int run_codegen_and_return();

// register allocation
a_register get_free_register();
void dealocate_reg(a_register reg);
void set_final_destination(a_register reg);
a_register final_destination;

//opcodes
void load_int_to_register(int imm, a_register reg);
void move_register_to_register(a_register reg_0, a_register reg_1);
void add_register_to_register(a_register reg_0, a_register reg_1);
void subtract_register_from_register(a_register reg_0, a_register reg_1);
void multiply_register_to_register(a_register reg_0, a_register reg_1);
void divide_register_by_register(a_register reg_0, a_register reg_1);
void prepare_return();
#endif