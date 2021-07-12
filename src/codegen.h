#ifndef CODEGEN
#define CODEGEN
extern unsigned char *codegen_mem;
extern int codegen_mem_offset;

#if defined(_M_X64) || defined(__x86_64__)
// x64
#include "x64/codegen-x64.h"
#elif defined(__AARCH64EL__) || defined(_M_ARM64)
// ARM64
#include "arm64/codegen-arm64.h"
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

// helpers
void init_codegen();
int run_codegen_and_return(fpr reg_result);

// register allocation
fpr allocate_fpr();
void dealocate_fpr(fpr reg);

// opcodes
void load_int_to_fpr(int imm, fpr reg_dst);
void negate_fpr(fpr reg_dst);
void add_fpr_to_fpr(fpr reg_dst, fpr reg_src);
void subtract_fpr_from_fpr(fpr reg_dst, fpr reg_src);
void multiply_fpr_to_fpr(fpr reg_dst, fpr reg_src);
void divide_fpr_by_fpr(fpr reg_dst, fpr reg_src);
void prepare_return(fpr reg_result);
#endif
