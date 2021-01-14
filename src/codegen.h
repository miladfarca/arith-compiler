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
void emit_imm(int immediate);
int run_codegen_and_return();

// register allocation
fpr allocate_fpr();
void dealocate_fpr(fpr reg);
void set_final_destination(fpr reg);
extern fpr final_destination;

//debuging
char *get_gpr_symbol(gpr reg);
char *get_fpr_symbol(fpr reg);
void print_inst(char *instr_symbol, int imm, char *reg_dst, char *reg_src, char *comments);

// opcodes
void load_int_to_fpr(int imm, fpr reg_dst);
void negate_fpr(fpr reg_dst);
void add_fpr_to_fpr(fpr reg_dst, fpr reg_src);
void subtract_fpr_from_fpr(fpr reg_dst, fpr reg_src);
void multiply_fpr_to_fpr(fpr reg_dst, fpr reg_src);
void divide_fpr_by_fpr(fpr reg_dst, fpr reg_src);
void prepare_return();
#endif
