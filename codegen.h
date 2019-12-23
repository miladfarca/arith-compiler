#ifndef CODEGEN
#define CODEGEN
void *codegen_mem;
int codegen_mem_offset;

//helpers
void init_codegen();
void emit(unsigned char byte);

//arch specific
// machine resgiters in order
typedef enum a_register
{
    //x64 register from 0 to 3
    eax,
    ecx,
    edx,
    ebx
} a_register;

// register allocation
a_register get_free_register();
void dealocate_reg(a_register reg);

//opcodes
void load_int_to_register(int imm, a_register reg);
void add_register_to_register(a_register reg_0, a_register reg_1);

void subtract_scratch_from_accum();

void multiply_register_to_register(a_register reg_0, a_register reg_1);

void divide_accum_by_scratch();
int run_codegen_and_return();
#endif