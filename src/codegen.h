#ifndef CODEGEN
#define CODEGEN
extern unsigned char *codegen_mem;
extern int codegen_mem_offset;

#if defined(_M_X64) || defined(__x86_64__)
// x64
#include "x64/codegen-x64.h"
#elif defined(__AARCH64EL__) || defined(_M_ARM64)
// ARM64
#error "Unimplemented!"
#elif defined(__mips64)
// MIPS64
#error "Unimplemented!"
#elif defined(__PPC64__) || defined(_ARCH_PPC64)
// PPC64
#error "Unimplemented!"
#elif defined(__s390__) || defined(__s390x__)
// S390x
#error "Unimplemented!"
#else
#error "Unknown architecture!"
#endif

//helpers
void init_codegen();
void emit(unsigned char byte);
int run_codegen_and_return();

// register allocation
a_register allocate_register();
void dealocate_reg(a_register reg);
void set_final_destination(a_register reg);
extern a_register final_destination;

//debuging
char *get_reg_symbol(a_register reg);
void print_inst(char *instr_symbol, int imm, a_register reg_dst, a_register reg_src);

//opcodes
void load_int_to_register(int imm, a_register reg_dst);
void negate_register(a_register reg_dst);
void move_register_to_register(a_register reg_dst, a_register reg_src);
void add_register_to_register(a_register reg_dst, a_register reg_src);
void subtract_register_from_register(a_register reg_dst, a_register reg_src);
void multiply_register_to_register(a_register reg_dst, a_register reg_src);
void divide_register_by_register(a_register reg_dst, a_register reg_src);
void prepare_return();
#endif